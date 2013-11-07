//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/EventHandling/UniqueEventHub.hpp"

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
class UniqueEvents
{
public:  
    typedef E Event;
    typedef K Key;

private:
    typedef AsioExpress::UniqueEventHub<E, K> EventHubType;
    typedef boost::shared_ptr<EventHubType> EventHubTypePointer;
    
public:
    class Listener
    {
        friend class UniqueEvents;
    public:
        Listener(UniqueEvents const & events) :
            eventHub(events.eventHub),
            eventValue(new typename EventHubType::Event)
        {
        }
            
        ///
        /// Creates a new listener for a specific event key. 
        /// @param key            The listener will receive events for this 
        ///                       specific key.
        ///
        void New(Key key)
        {            
            listener = eventHub->NewListener(key, eventValue);
        }

        ///
        /// Creates a new listener for the default event key. 
        /// @param eventValue     Pointer to an event value that will be set to the
        ///                       event value when it's received.
        ///
        void New()
        {
            listener = eventHub->NewListener(eventValue);
        }
        
        ///
        /// Call this to wait for an event listener to receive an event.
        /// @param waitTimer          A timer to indicate how long to wait to receive 
        ///                           the event.
        /// @param completionHandler  The completion hander is called when the event 
        ///                           is received, the wait time expires, or an error
        ///                           occurs.
        ///
        void AsyncWait(
            TimerPointer const & waitTimer, 
            CompletionHandler completionHandler)
        {
            eventHub->AsyncWait(listener, waitTimer, completionHandler);
        }
        
        void Cancel()
        {
            listener.reset();
        }
        
        Event GetEventValue() const
        {
            return *eventValue;
        }
        
    private:
        EventHubTypePointer                       eventHub;
        typename EventHubType::ListenerPointer    listener;
        typename EventHubType::EventPointer       eventValue;
    };

public:
    UniqueEvents() :
        eventHub(new EventHubType)
    {
    }
    
    ///
    /// This method adds an event for a specific event key.
    /// @param key            The event is added to the event queue for this 
    ///                       specific key.
    /// @param eventValue     The event value that will be received by the 
    ///                       waiting event listener.
    ///
    void Add(
        Key key,
        Event eventValue)
    {
        eventHub->Add(key, eventValue);
    }

    ///
    /// This method adds an event for the default event key.
    /// @param eventValue      The event value that will be received by the 
    ///                       waiting event listener.
    ///
    void Add(
        Event eventValue)
    {
        eventHub->Add(eventValue);
    }

    ///
    /// This method cancels any pending operations. If AsyncWait is called
    /// an operation aborted error is returned immediately.
    ///
    void ShutDown()
    {
        eventHub->ShutDown();
    }

private:
    EventHubTypePointer  eventHub;
};

} // namespace AsioExpress
