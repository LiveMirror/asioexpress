//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/EventHandling/TaskPool.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"
#include "AsioExpress/Testing/SetUnitTestMode.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace AsioExpress::TaskPoolPrivate;
using namespace std;

BOOST_AUTO_TEST_SUITE(TaskPoolTest)

class MyEventHandler
{
public:
    MyEventHandler() :
        m_callCount(new int()),
        m_event(new string)
    {
    }

    void operator()(std::string const & ev, CompletionHandler completionHandler)
    {
        ++(*m_callCount);
        *m_event = ev;
        completionHandler(AsioExpress::Error());
    }

    int GetCallCount() const
    {
        return *m_callCount;
    }

    std::string GetEvent() const
    {
        return *m_event;
    }

private:
    boost::shared_ptr<int>                  m_callCount;
    boost::shared_ptr<std::string>          m_event;
};

class MyBadEventHandler1
{
public:
    void operator()(std::string const & ev, CompletionHandler completionHandler)
    {
        completionHandler(AsioExpress::Error(AsioExpress::ErrorCode::UniqueEventTimeout));
    }
};

class MyBadEventHandler2
{
public:
    void operator()(std::string const & ev, CompletionHandler completionHandler)
    {
        throw AsioExpress::ContractViolationException("Programmer error!");
    }
};

BOOST_AUTO_TEST_CASE(TestTaskPoolAdd)
{
    SetUnitTestMode(true);

    boost::asio::io_service ioService;
    AutoCompletionHandler errorHandler(__FILE__,__LINE__);
    MyEventHandler eventHandler;

    TaskPool<std::string,MyEventHandler> taskPool(ioService, 1, eventHandler);
    taskPool.SetErrorHandler(errorHandler);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    taskPool.AsyncAdd(std::string("hello"), addHandler);

    taskPool.TestRead();

    BOOST_CHECK_EQUAL(addHandler.Calls(), 1);
    BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 1);
    BOOST_CHECK_EQUAL(eventHandler.GetEvent(), "hello");
}

BOOST_AUTO_TEST_CASE(TestTaskPoolLoop)
{
    SetUnitTestMode(true);

    boost::asio::io_service ioService;
    AutoCompletionHandler errorHandler(__FILE__,__LINE__);
    MyEventHandler eventHandler;

    TaskPool<std::string,MyEventHandler> taskPool(ioService, 2, eventHandler);
    taskPool.SetErrorHandler(errorHandler);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);

    taskPool.AsyncAdd(std::string("hello"), addHandler);
    taskPool.TestRead();

    taskPool.AsyncAdd(std::string("there"), addHandler);
    taskPool.TestRead();

    BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 2);
}

BOOST_AUTO_TEST_CASE(TestBadEventHandler1)
{
    SetUnitTestMode(true);

    boost::asio::io_service ioService;
    TestCompletionHandler errorHandler;
    MyBadEventHandler1 eventHandler;

    TaskPool<std::string,MyBadEventHandler1> taskPool(ioService, 3, eventHandler);
    taskPool.SetErrorHandler(errorHandler);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    taskPool.AsyncAdd(std::string("hello"), addHandler);

    taskPool.TestRead();

    BOOST_CHECK(errorHandler.LastError().GetErrorCode() ==
                AsioExpress::ErrorCode::UniqueEventTimeout);
}

BOOST_AUTO_TEST_CASE(TestBadEventHandler2)
{
    SetUnitTestMode(true);

    boost::asio::io_service ioService;
    TestCompletionHandler errorHandler;
    MyBadEventHandler2 eventHandler;

    TaskPool<std::string,MyBadEventHandler2> taskPool(ioService, 4, eventHandler);
    taskPool.SetErrorHandler(errorHandler);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    taskPool.AsyncAdd(std::string("hello"), addHandler);

    taskPool.TestRead();

    BOOST_CHECK(errorHandler.LastError().GetErrorCode() ==
                AsioExpress::Common::ErrorCode::ContractViolation);
}

BOOST_AUTO_TEST_SUITE_END()
