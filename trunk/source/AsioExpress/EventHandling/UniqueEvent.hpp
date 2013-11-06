//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/Check.hpp"

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/EventHandling/UniqueEventPrivate/UniqueEventListener.hpp"

namespace AsioExpress {

///
/// This class allows a process to wait on an event: 
///   - It relies upon the creation of a listener object to receive the event. 
///   - An event consumer receives an event by calling AsyncWait for an
///     individual listener.
///   - Events are raised by call to Add.
///   - If there is no listener for a raised event then it is not queued for 
///     reuse. This protects the consumer from consuming stale events.
///  
/// This event primitive is typically used in the following way:
///    - The process creates a listener for the event.
///    - The process starts an asynchronous operation (typically a service 
///      request).
///    - The process calls AsyncWait to wait for the response/result from the
///      asynchronous operation.
///    - When the response/result is available the Add method is called to 
///      raise the event.
///
///  The creation of the listener defines the life-cycle for the event consumer. 
///  It is important that the listener is created before initiating the 
///  asynchronous operation because the operation may complete before the 
///  process reaches the call to AsyncWait. The creation of the listener 
///  guarantees any events raised during its lifetime are captured.
///
template<typename E, typename K = int>
class UniqueEvent
{
public:
  typedef E Event;
  typedef K Key;
  typedef boost::shared_ptr<Event> EventPointer;

private:
  typedef UniqueEventPrivate::UniqueEventListener<E> UniqueEventListener; 
  typedef boost::shared_ptr<UniqueEventListener> UniqueEventListenerPointer;

  typedef std::map<Key,UniqueEventListenerPointer> Listeners;
  typedef boost::shared_ptr<Listeners> ListenersPointer;

public:
  class Listener
  {
    friend class UniqueEvent;
  private:
    Listener(
        Key key,
        UniqueEventListenerPointer eventListener,
        ListenersPointer listeners) :
      m_key(key),
      m_listener(eventListener),
      m_listeners(listeners)
    {
    }
    void AsyncWait(
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler)
    {
      m_listener->AsyncWait(waitTimer, completionHandler);
    }
  public:
    void Cancel()
    {
      // Cancel the event listener.
      m_listener->Cancel();
      m_listeners->erase(m_key);
    }
    ~Listener()
    {
      Cancel();
    }
  private:
    Key                         m_key;
    UniqueEventListenerPointer  m_listener;
    ListenersPointer            m_listeners;
  };
  typedef boost::shared_ptr<Listener> ListenerPointer;

public:
  UniqueEvent();

  ///
  /// Creates a new listener for a specific event key. The event value is copied 
  /// to the output parameter when the event is received.
  /// @param key            The listener will receive events for this 
  ///                       specific key.
  /// @param eventValue     Pointer to an event value that will be set to the
  ///                       event value when it's received.
  ///
  ListenerPointer NewListener(
      Key key,
      EventPointer const & eventValue);

  ///
  /// Creates a new listener for the default event key. The event value is copied 
  /// to the output parameter when the event is received.
  /// @param eventValue     Pointer to an event value that will be set to the
  ///                       event value when it's received.
  ///
  ListenerPointer NewListener(
      EventPointer const & eventValue);

  ///
  /// Call this to wait for an event listener to receive an event.
  /// @param listener           Event listener pointer.
  /// @param waitTimer          A timer to indicate how long to wait to receive 
  ///                           the event.
  /// @param completionHandler  The completion hander is called when the event 
  ///                           is received, the wait time expires, or an error
  ///                           occurs.
  ///
  void AsyncWait(
      ListenerPointer const & listener,
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// This method adds an event for a specific event key.
  /// @param key            The event is added to the event queue for this 
  ///                       specific key.
  /// @param eventValue     The event value that will be received by the 
  ///                       waiting event listener.
  ///
  void Add(
      Key key,
      Event eventValue);

  ///
  /// This method adds an event for the default event key.
  /// @param eventValue      The event value that will be received by the 
  ///                       waiting event listener.
  ///
  void Add(
      Event eventValue);

  ///
  /// This method cancels any pending operations. If AsyncWait is called
  /// an operation aborted error is retured immediately.
  ///
  void ShutDown();

private:
  ListenersPointer   m_eventListeners;
  bool               m_isShutDown;
};

template<typename E, typename K>
inline UniqueEvent<E,K>::UniqueEvent() :
  m_eventListeners(new Listeners),
  m_isShutDown(false)
{
}

template<typename E, typename K>
inline typename UniqueEvent<E,K>::ListenerPointer UniqueEvent<E,K>::NewListener(
      Key key,
      EventPointer const & eventValue)
{
  CHECK_MSG(m_eventListeners->count(key) == 0, "A UniqueEvent listener with this key already exists.");

  UniqueEventListenerPointer eventListener(new UniqueEventListener(eventValue));

  (*m_eventListeners)[key] = eventListener;

  return ListenerPointer(new Listener(key, eventListener, m_eventListeners));
}

template<typename E, typename K>
inline typename UniqueEvent<E,K>::ListenerPointer UniqueEvent<E,K>::NewListener(
      EventPointer const & eventValue)
{
  return NewListener(Key(), eventValue);
}

template<typename E, typename K>
inline void UniqueEvent<E,K>::AsyncWait(
      ListenerPointer const & listener,
      TimerPointer const & waitTimer, 
      CompletionHandler completionHandler)
{
  CHECK(listener);
  CHECK(waitTimer);
  CHECK(!completionHandler.empty());

  // If the event has been canceled return operation aborted immediately.
  if ( m_isShutDown )
  {
    listener->Cancel();
    completionHandler(Error(boost::asio::error::operation_aborted));
    return;
  }

  listener->AsyncWait(waitTimer, completionHandler);
}

template<typename E, typename K>
inline void UniqueEvent<E,K>::Add(
      Key key,
      Event eventValue)
{
  typename Listeners::iterator eventListener = m_eventListeners->find(key);

  // If the listener has timed-out it will not be found and is not an error.
  // It just means this event was devlivered too late.
  if (eventListener != m_eventListeners->end())
    eventListener->second->AddEvent(eventValue);
}

template<typename E, typename K>
inline void UniqueEvent<E,K>::Add(
      Event eventValue)
{
  Add(Key(), eventValue);
}

template<typename E, typename K>
inline void UniqueEvent<E,K>::ShutDown()
{
  // Indicate that this event is canceled.
  m_isShutDown = true;

  // Cancel all waiting listeners.
  BOOST_FOREACH( typename Listeners::value_type &it, *m_eventListeners )
  {
    UniqueEventListenerPointer eventListener(it.second);
    eventListener->Cancel();
  }
}

} // namespace AsioExpress
