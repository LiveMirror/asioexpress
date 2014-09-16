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

#include "AsioExpress/PointerTypes.hpp"
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
class UniqueEvents
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

    ///
    /// The deleter is responsible for removing the listener from the map.
    ///
    class ListenerDeleter
    {
    public:
        ListenerDeleter(Key key, ListenersPointer listeners) :
            key(key),
            listeners(listeners)
        {
        }

        ~ListenerDeleter()
        {
            listeners->at(key)->Cancel();
            listeners->erase(key);
        }

    private:
        Key                 key;
        ListenersPointer    listeners;
    };
    typedef boost::shared_ptr<ListenerDeleter> ListenerDeleterPointer;

public:
    typedef typename Listeners::size_type SizeType;

    ///
    /// Use a listener object to wait upon unique events.
    ///
    class Listener
    {
        friend class UniqueEvents;
    public:
        ///
        /// Construct a listener to listen for events added to the provided
        /// unique events object.
        ///
        Listener(UniqueEvents & events) :
            listeners(events.eventListeners),
            isShutDown(events.isShutDown),
            eventValue(new typename UniqueEvents::Event)
        {
        }

        ///
        /// Creates a new listener for a specific event key.
        /// @param key      The listener will receive events for this
        ///                  specific key.
        ///
        void New(Key key)
        {
            CHECK_MSG(
                listeners->count(key) == 0,
                "A UniqueEventHub listener with this key already exists.");

            listener.reset(new UniqueEventListener(eventValue));
            listenerDeleter.reset(new ListenerDeleter(key,listeners));

            (*listeners)[key] = listener;
        }

        ///
        /// Creates a new listener for the default event key.
        ///
        void New()
        {
            New(Key());
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
            CHECK(listener);
            CHECK(waitTimer);
            CHECK(!completionHandler.empty());

            // If the event has been canceled return operation aborted immediately.
            if ( *isShutDown )
            {
                listener->Cancel();
                completionHandler(Error(boost::asio::error::operation_aborted));
                return;
            }

            listener->AsyncWait(waitTimer, completionHandler);
        }

        ///
        /// Cancel waiting for an event.
        ///
        void Cancel()
        {
            listener->Cancel();
            // delete local reference
            listener.reset();
            listenerDeleter.reset();
        }

        ///
        /// Get the event value.
        /// @returns Returns the event value after a successful AsyncWait.
        ///
        Event GetEventValue() const
        {
            return *eventValue;
        }

    private:
        Listener & operator=(Listener const &);

        //
        // Implementation Notes:
        // There is an internal event listener object (UniqueEventListener) that
        // handles the event operations: add, wait, cancel, and timeout.
        // This class controls the lifetime of the internal event
        // listeners in event listeners map owned by UniqueEvents.
        //

        // These pointers are copied from the parent UniqueEvents object.
        ListenersPointer                    listeners;
        BoolPointer                         isShutDown;

        // Creates a unique events listener through the New calls and
        // also creates a deleter for it.
        UniqueEventListenerPointer          listener;
        ListenerDeleterPointer              listenerDeleter;

        // The unique events listener writes events to this value.
        typename UniqueEvents::EventPointer eventValue;
    };

public:
    UniqueEvents() :
        eventListeners(new Listeners),
        isShutDown(new bool(false))
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
        typename Listeners::iterator eventListener = eventListeners->find(key);

        // If the listener has timed-out it will not be found and is not an error.
        // It just means this event was delivered too late.
        if (eventListener != eventListeners->end())
            eventListener->second->AddEvent(eventValue);
    }

    ///
    /// This method adds an event for the default event key.
    /// @param eventValue      The event value that will be received by the
    ///                       waiting event listener.
    ///
    void Add(
        Event eventValue)
    {
        Add(Key(), eventValue);
    }

    ///
    /// This method cancels any pending operations. If AsyncWait is called
    /// an operation aborted error is returned immediately.
    ///
    void ShutDown()
    {
        // Indicate that this event is canceled.
        *isShutDown = true;

        // Cancel all waiting listeners.
        BOOST_FOREACH( typename Listeners::value_type &it, *eventListeners )
        {
            UniqueEventListenerPointer eventListener(it.second);
            eventListener->Cancel();
        }
    }

    ///
    /// Call to get the number of listeners waiting for events.
    /// Used for testing that the listeners map is properly maintained.
    ///
    SizeType GetListenerCount()
    {
        return eventListeners->size();
    }

private:
  ListenersPointer   eventListeners;
  BoolPointer        isShutDown;
};

} // namespace AsioExpress
