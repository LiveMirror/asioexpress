//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <deque>

#include <boost/shared_ptr.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/CatchMacros.hpp"

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "AsioExpress/Timer/NoExpiryTimer.hpp"

#include "AsioExpress/EventHandling/EventQueue.hpp"

#include "AsioExpress/Testing/SetUnitTestMode.hpp"

namespace AsioExpress {
namespace TaskPoolPrivate {

///
/// The task pool mock is used to simulate the behavior of the task pool.
///
template<typename E, typename H>
class TaskPoolMock
{
public:
    TaskPoolMock(
            H eventHandler,
            AsioExpress::CompletionHandler errorHandler) :
        eventHandler(eventHandler),
        errorHandler(errorHandler)
    {
    }

    void SetErrorHandler(AsioExpress::CompletionHandler newErrorHandler)
    {
        errorHandler = newErrorHandler;
    }

    void AsyncAdd(
        E const & event,
        CompletionHandler completionHandler)
    {
        eventQueue.push_back(event);
        completionHandler(AsioExpress::Error());
    }

    void TestRead()
    {
        CHECK_MSG(g_isUnitTestMode, "Only use this method for unit testing.");

        // Makes a copy of the provided event handler.
        H handler(eventHandler);
        try
        {
            E event = eventQueue.front();
            handler(event, boost::bind(&TaskPoolMock::ErrorHandler, this, _1));
            eventQueue.pop_front();
        }
        ASIOEXPRESS_CATCH_ERROR_AND_DO(errorHandler(error))
    }

private:
    void ErrorHandler(AsioExpress::Error error)
    {
        if (error)
            errorHandler(error);
    }

    typedef std::deque<E>               EventQueue;
    EventQueue                          eventQueue;
    H                                   eventHandler;
    AsioExpress::CompletionHandler      errorHandler;
};

} // namespace TaskPoolPrivate
} // namespace AsioExpress
