//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/Callstack.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Timer/Timer.hpp"

namespace AsioExpress {

///
/// This is a basic event synchronization primitive for ASIO
/// applications.
/// 
template<typename E, typename Key = int>
class EventQueue
{
public:
  typedef E Event;
  typedef boost::shared_ptr<Event> EventPointer;

  EventQueue() :
    m_isShutDown(false)
  {
  }

  ~EventQueue()
  {
  }

  ///
  /// Get the next event from the event queue. If no events are queued the 
  /// async call waits for a new event to be added to the queue.
  ///
  /// @param key                The caller will receive events for this 
  ///                           specific key.
  /// @param event              The event data that will be received by the 
  ///                           caller.
  /// @param waitTimer          A timer to indicate how long to wait to receive 
  ///                           the event.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is received, the wait time expires, or an error
  ///                           occurs.
  void AsyncWait(
      Key key,
      EventPointer event, 
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// Get the next event from the event queue. If no events are queued the 
  /// async call waits for a new event to be added to the queue.
  ///
  /// @param event              The event data that will be received by the 
  ///                           caller.
  /// @param waitTimer          A timer to indicate how long to wait to receive 
  ///                           the event.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is received, the wait time expires, or an error
  ///                           occurs.
  void AsyncWait(
      EventPointer event, 
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// This method adds an event to the event queue for a specific event key.
  ///
  /// @param key                The event is added to the event queue for this 
  ///                           specific key.
  /// @param event              The event data that will be received by the 
  ///                           caller.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is queued or the queue has been canceled.
  ///
  void AsyncAdd(
      Key key,
      Event event,
      CompletionHandler completionHandler);

  ///
  /// This method adds an event to the event queue for a specific event key.
  ///
  /// @param event              The event data that will be received by the 
  ///                           caller.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is queued or the queue has been canceled.
  ///
  void AsyncAdd(
      Event event,
      CompletionHandler completionHandler);

  ///
  /// This method cancels all pending operations on the event queue. If
  /// AsyncWait or AsyncAdd is called on a canceled queue an operation
  /// aborted error is retured immediately.
  ///
  void ShutDown();

private:
  Error Add(
      Key key,
      Event event);

  void Timeout(
      Error error, 
      TimerPointer const & timer);

  struct EventHandler
  {
    EventHandler(
        Key key,
        EventPointer event, 
        CompletionHandler completionHandler,
        TimerPointer const & waitTimer) :
      key(key),
      event(event),
      completionHandler(completionHandler),
      timer(waitTimer)
    {
    }
    Key                 key;
    EventPointer        event;
    CompletionHandler   completionHandler;
    TimerPointer        timer;
  };

  struct RegisteredEvent
  {
    RegisteredEvent(
        Key key,
        Event event) :
      key(key),
      event(event)
    {
    }
    Key     key;
    Event   event;
  };

  typedef std::vector<EventHandler> EventHandlerList;
  typedef std::vector<RegisteredEvent> RegisteredEvents;
  typedef std::set<Key> KeySet;

  EventHandlerList      m_waitingEventHandlers;
  RegisteredEvents      m_registeredEvents;
  bool                  m_isShutDown;
};

template<typename Event, typename Key>
void EventQueue<Event, Key>::AsyncWait(
      Key key,
      EventPointer event, 
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler)
{
  // If the queue has been canceled return operation aborted immediately.
  if ( m_isShutDown )
  {
    completionHandler(Error(boost::asio::error::operation_aborted));
    return;
  }

  // Verify that we have a unique event and timer pointer.
  {
    EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
    EventHandlerList::iterator end = m_waitingEventHandlers.end();
    for (; it != end; ++it)
    {
      CHECK_MSG(it->event != event, "Outstanding wait for this event pointer in call to EventQueue::AsyncWait.");
      CHECK_MSG(it->timer != waitTimer, "Outstanding wait for this timer in call to EventQueue::AsyncWait.");
    }
  }

  // Look up waiting event.
  {
    RegisteredEvents::iterator  it = m_registeredEvents.begin();
    RegisteredEvents::iterator end = m_registeredEvents.end();
    for (; it != end; ++it)
    {
      if (it->key == key)
      {
        *event = it->event;
        m_registeredEvents.erase(it);
        completionHandler(Error());
        return;
      }
    }
  }

  // Queue this event to be handled. 
  //
  waitTimer->AsyncWait(boost::bind(&EventQueue::Timeout, this, _1, waitTimer));

  m_waitingEventHandlers.push_back(
    EventHandler(key, event, completionHandler, waitTimer));
}

template<typename Event, typename Key>
void EventQueue<Event, Key>::AsyncWait(
      EventPointer event, 
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler)
{
  AsyncWait(Key(), event, waitTimer, completionHandler);
}

template<typename Event, typename Key>
void EventQueue<Event, Key>::AsyncAdd(
      Key key,
      Event event,
      CompletionHandler completionHandler)
{
  // If the queue has been canceled return operation aborted immediately.
  if ( m_isShutDown )
  {
    completionHandler(Error(boost::asio::error::operation_aborted));
    return;
  }

  completionHandler(Add(key,event));
}

template<typename Event, typename Key>
Error EventQueue<Event, Key>::Add(
      Key key,
      Event event)
{
  // Look up waiting handler.
  EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
  EventHandlerList::iterator end = m_waitingEventHandlers.end();
  for (; it != end; ++it)
  {
    if (it->key == key)
    {
      it->timer->Stop();
      *(it->event) = event;
      CompletionHandler handler(it->completionHandler);
      m_waitingEventHandlers.erase(it);
      handler(Error());
      return Error();
    }
  }

  // No handler found so we put it in event queue.
  m_registeredEvents.push_back(RegisteredEvent(key, event));

  return Error();
}

template<typename Event, typename Key>
void EventQueue<Event, Key>::AsyncAdd(
      Event event,
      CompletionHandler completionHandler)
{
  AsyncAdd(Key(), event, completionHandler);
}

template<typename Event, typename Key>
void EventQueue<Event, Key>::ShutDown()
{
  // Indicate that this queue is canceled.
  m_isShutDown = true;

  // Look up waiting handler.
  EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
  EventHandlerList::iterator end = m_waitingEventHandlers.end();
  for (; it != end; ++it)
  {
    it->timer->Stop();
  }
}

template<typename Event, typename Key>
void EventQueue<Event, Key>::Timeout(
    Error error, 
    TimerPointer const & timer)
{
  // Convert to timeout error if timer has expired.
  if (!error)
    error = Error(ErrorCode::EventQueueTimeout);

  EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
  EventHandlerList::iterator end = m_waitingEventHandlers.end();
  for (; it != end; ++it)
  {
    if (it->timer == timer)
    {
      CompletionHandler handler(it->completionHandler);
      m_waitingEventHandlers.erase(it);
      handler(error);
      break;
    }
  }
}

} // namespace AsioExpress
