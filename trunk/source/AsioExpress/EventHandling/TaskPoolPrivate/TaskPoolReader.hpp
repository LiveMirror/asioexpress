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
    typedef EventQueue<E> Queue;
    typedef boost::shared_ptr<Queue> QueuePointer;

    TaskPoolReader(
            QueuePointer const & eventQueue,
            TimerPointer const & waitTimer,
            H eventHandler,
            AsioExpress::CompletionHandler errorHandler) :
        eventQueue(eventQueue),
        waitTimer(waitTimer),
        eventPointer(new E),
        eventHandler(eventHandler),
        errorHandler(errorHandler)
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
                    errorHandler(error);
                }

                YIELD
                {
                    // Makes a copy of the provided event handler.
                    H handler(eventHandler);
                    try
                    {
                        handler(*eventPointer, *this);
                    }
                    ASIOEXPRESS_CATCH_ERROR_AND_DO(errorHandler(error))
                }
                
                if (error)
                    errorHandler(error);
            }
        }
    }

private:
    QueuePointer                        eventQueue;
    TimerPointer                        waitTimer;
    typename Queue::EventPointer        eventPointer;
    H                                   eventHandler;
    AsioExpress::CompletionHandler      errorHandler;
};

} // namespace TaskPoolPrivate
} // namespace AsioExpress
