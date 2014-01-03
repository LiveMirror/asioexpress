//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <set>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/CallStack.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Timer/Timer.hpp"

#undef max 

namespace AsioExpress {

///
/// This is a basic event synchronization primitive for ASIO applications.
/// The EventQueue allows you to decouple the event processor from the async
/// processes generating the events. The queue accepts a maximum size parameter
/// that will block the completion of AsyncAdd to prevent the event queue from
/// getting unreasonably large and providing feedback to the rest of the 
/// system.
///

template<typename E>
class EventQueue
{
public:
  typedef E Event;
  typedef boost::shared_ptr<Event> EventPointer;
  typedef size_t SizeType;

  EventQueue() :
    m_isShutDown(false),
    m_maxSize(std::numeric_limits<SizeType>::max())
  {
  }

  EventQueue(SizeType size) :
    m_isShutDown(false),
    m_maxSize(size)
  {
      CHECK(size > 0);
  }

  ~EventQueue()
  {
  }

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
      EventPointer const & event, 
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// This method adds an event to the event queue.
  ///
  /// @param event              The event data that will be received by the 
  ///                           caller.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is queued or the queue has been canceled.
  ///
  void AsyncAdd(
      Event const & event,
      CompletionHandler completionHandler);

  ///
  /// This method cancels all pending operations on the event queue. If
  /// AsyncWait or AsyncAdd is called on a canceled queue an operation
  /// aborted error is retured immediately.
  ///
  void ShutDown();

private:
  void Timeout(
      Error error, 
      TimerPointer const & timer);

  struct EventHandler
  {
    EventHandler(
        EventPointer const & event, 
        CompletionHandler const & completionHandler,
        TimerPointer const & waitTimer) :
      event(event),
      completionHandler(completionHandler),
      timer(waitTimer)
    {
    }
    EventPointer        event;
    CompletionHandler   completionHandler;
    TimerPointer        timer;
  };

  struct WaitingEvent
  {
    WaitingEvent(
        Event const & event, 
        CompletionHandler const & completionHandler) :
      waitingEvent(event),
      completionHandler(completionHandler)
    {
    }
    Event               waitingEvent;
    CompletionHandler   completionHandler;
  };

  typedef std::vector<EventHandler> EventHandlerList;
  typedef std::vector<Event> RegisteredEvents;
  typedef std::vector<WaitingEvent> WaitingEvents;

  EventHandlerList      m_waitingEventHandlers;
  RegisteredEvents      m_registeredEvents;
  WaitingEvents         m_waitingEvents;
  bool                  m_isShutDown;
  SizeType              m_maxSize;
};

template<typename Event>
void EventQueue<Event>::AsyncWait(
      EventPointer const & event, 
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
    typename EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
    typename EventHandlerList::iterator end = m_waitingEventHandlers.end();
    for (; it != end; ++it)
    {
      CHECK_MSG(it->event != event, "Outstanding wait for this event pointer in call to EventQueue::AsyncWait.");
      CHECK_MSG(it->timer != waitTimer, "Outstanding wait for this timer in call to EventQueue::AsyncWait.");
    }
  }

  // Look up waiting event.
  if (! m_registeredEvents.empty())
  {
    typename RegisteredEvents::iterator  it = m_registeredEvents.begin();
    *event = *it;
    m_registeredEvents.erase(it);
    completionHandler(Error());

    // move waiting event to registered event
    if (! m_waitingEvents.empty())
    {      
      typename WaitingEvents::iterator it = m_waitingEvents.begin();
      m_registeredEvents.push_back(it->waitingEvent);
      CompletionHandler handler(it->completionHandler);
      m_waitingEvents.erase(it);
      handler(Error());
    }
    return;
  } 

  // Queue this event to be handled. 
  //
  waitTimer->AsyncWait(boost::bind(&EventQueue::Timeout, this, _1, waitTimer));

  m_waitingEventHandlers.push_back(
    EventHandler(event, completionHandler, waitTimer));
}

template<typename Event>
void EventQueue<Event>::AsyncAdd(
      Event const & event,
      CompletionHandler completionHandler)
{
  // If the queue has been canceled return operation aborted immediately.
  if ( m_isShutDown )
  {
    completionHandler(Error(boost::asio::error::operation_aborted));
    return;
  }

  if(! m_waitingEventHandlers.empty())
  {
    // Look up waiting handler.
    typename EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
    it->timer->Stop();
    *(it->event) = event;
    CompletionHandler handler(it->completionHandler);
    m_waitingEventHandlers.erase(it);
    handler(Error());
    completionHandler(Error());
    return;
  }

  if (m_registeredEvents.size() >= m_maxSize)
  {
      // Add event and this completion hander to the following queue.
      m_waitingEvents.push_back(WaitingEvent(event,completionHandler));
      return;
  }

  // No handler found so we put it in the event queue.
  m_registeredEvents.push_back(event);
  completionHandler(Error());
}

template<typename Event>
void EventQueue<Event>::ShutDown()
{
  // Indicate that this queue is canceled.
  m_isShutDown = true;

  // Look up waiting handler.
  typename EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
  typename EventHandlerList::iterator end = m_waitingEventHandlers.end();
  for (; it != end; ++it)
  {
    it->timer->Stop();
  }
}

template<typename Event>
void EventQueue<Event>::Timeout(
    Error error, 
    TimerPointer const & timer)
{
  // Convert to timeout error if timer has expired.
  if (!error)
    error = Error(ErrorCode::EventQueueTimeout);

  typename EventHandlerList::iterator  it = m_waitingEventHandlers.begin();
  typename EventHandlerList::iterator end = m_waitingEventHandlers.end();
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
