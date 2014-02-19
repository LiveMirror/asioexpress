//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/CallStack.hpp"

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "AsioExpress/Proc/Context.hpp"

#include "AsioExpress/EventHandling/EventQueue.hpp"
#include "AsioExpress/EventHandling/TaskPoolPrivate/TaskPoolReader.hpp"
#include "AsioExpress/EventHandling/TaskPoolPrivate/TaskPoolMock.hpp"

#include "AsioExpress/Testing/SetUnitTestMode.hpp"

namespace AsioExpress {

///
/// This is a basic event synchronization primitive for ASIO applications.
/// TaskPool is an extension of the EventQueue primitive. As well as providing
/// a basic queue for processing events, it allows a fixed number of events to
/// be processed simultaneously. It works similar to the concept of a thread
/// pool but it is using asynchronous tasks rather than threads.
///
template<typename E, typename H>
class TaskPool
{
public:
    typedef E Event;
    typedef boost::shared_ptr<Event> EventPointer;
    typedef size_t SizeType;

    TaskPool(
            boost::asio::io_service & ioService,
            SizeType poolSize,
            H eventHandler) :
        ioService(ioService),
        eventQueue(new Queue),
        poolSize(poolSize),
        eventHandler(eventHandler),
        started(false),
        mock(eventHandler, errorHandler)
    {
        CHECK(poolSize > 0);
    }

    TaskPool(
            boost::asio::io_service & ioService,
            SizeType poolSize,
            SizeType queueSize,
            H eventHandler) :
        ioService(ioService),
        eventQueue(new Queue(queueSize)),
        poolSize(poolSize),
        eventHandler(eventHandler),
        errorHandler(NullCompletionHandler),
        mock(eventHandler, errorHandler)
    {
        CHECK(poolSize > 0);
    }

    ///
    /// Call this method to set an error hander for the task pool. Any errors
    /// generated from the event handler will passed to the error handler.
    ///
    void SetErrorHandler(AsioExpress::CompletionHandler newErrorHandler)
    {
        errorHandler = newErrorHandler;
        mock.SetErrorHandler(newErrorHandler);
    }

    ///

    /// Call this method to start the task pool.
    ///
    void Start()
    {
        CHECK(!started);

        if (g_isUnitTestMode)
        {
            started = true;
            return;
        }
        
        for (size_t i=0; i<poolSize; ++i)
        {
            ioService.post(boost::asio::detail::bind_handler(
                TaskPoolPrivate::TaskPoolReader<E,H>(eventQueue, TimerPointer(new NoExpiryTimer(ioService)), eventHandler, errorHandler),
                AsioExpress::Error()));
        }

        started = true;
    }

    ///
    /// Call this method to stop the task pool.
    ///
    void Stop()
    {
        CHECK(started);

        eventQueue->Cancel();

        started = false;
    }

    ///
    /// This method adds an event to the task pool.
    ///
    /// @param event              The event data that will be received by the
    ///                           caller.
    /// @param completionHandler  The completion hander is called when the event
    ///                           is queued or the queue has been canceled.
    ///
    void AsyncAdd(
            Event const & event,
            CompletionHandler completionHandler)
    {
        if (g_isUnitTestMode)
        {
            mock.AsyncAdd(event, completionHandler);
            return;
        }

        eventQueue->AsyncAdd(event, completionHandler);
    }

    ///
    /// Unit testing method allows events to be manually read and handled by
    /// the task pool.
    ///
    void TestRead()
    {
        mock.TestRead();
    }

    ///
    /// This method cancels all pending operations on the task pool. If
    /// AsyncWait or AsyncAdd is called on a canceled queue an operation
    /// aborted error is returned immediately.
    ///
    void ShutDown()
    {
        eventQueue->ShutDown();
    }

private:
    typedef EventQueue<E> Queue;
    typedef boost::shared_ptr<Queue> QueuePointer;

    boost::asio::io_service &           ioService;
    QueuePointer                        eventQueue;
    SizeType                            poolSize;
    H                                   eventHandler;
    AsioExpress::CompletionHandler      errorHandler;
    bool                                started;
    TaskPoolPrivate::TaskPoolMock<E,H>  mock;
};

} // namespace AsioExpress
