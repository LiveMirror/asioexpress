//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>

#include <boost/shared_ptr.hpp>

#include "AsioExpressError/Check.hpp"

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "AsioExpress/Timer/NoExpiryTimer.hpp"

#include "AsioExpress/EventHandling/EventQueue.hpp"

#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace TaskPoolPrivate {

///
/// The task pool reader endlessly waits upon events from the event queue and
/// executes the event hander when new events arrive.
/// 
template<typename E, typename H>
class TaskPoolReader : private AsioExpress::Coroutine
{
public:
    typedef EventQueue<E> EventQueue;
    typedef boost::shared_ptr<EventQueue> EventQueuePointer;

    TaskPoolReader(
            EventQueuePointer const & eventQueue,
            TimerPointer const & waitTimer,
            H eventHandler) :
        eventQueue(eventQueue),
        waitTimer(waitTimer),
        eventPointer(new E),
        eventHandler(eventHandler)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
        REENTER (this)
        {
            for(;;)
            {
                YIELD
                {
                    eventQueue->AsyncWait(eventPointer, waitTimer, *this);
                }

                if (error.GetErrorCode() == boost::asio::error::operation_aborted)
                    break;

                if (error)
                {
                    // should never happen
                    assert(false);
                    abort();
                }

                YIELD
                {
                    // Makes a copy of the provided event handler.
                    H handler(eventHandler);
                    try
                    {
                        handler(eventPointer, *this);
                    }
                    catch(...)
                    {
                        // Do nothing; the task pool does not handle errors with
                        // the user handler.
                    }
                }
            }
        }
    }

private:
    EventQueuePointer                   eventQueue;
    TimerPointer                        waitTimer;
    typename EventQueue::EventPointer   eventPointer;
    H                                   eventHandler;
};

} // namespace TaskPoolPrivate
} // namespace AsioExpress
