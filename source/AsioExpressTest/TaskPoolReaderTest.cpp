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

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace AsioExpress::TaskPoolPrivate;
using namespace std;

BOOST_AUTO_TEST_SUITE(TaskPoolReaderTest)

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

BOOST_AUTO_TEST_CASE(TestTaskPoolReaderWait)
{
  typedef EventQueue<std::string> TestQueue;
  typedef boost::shared_ptr<TestQueue> TestQueuePointer;

  TestQueuePointer testQueue(new TestQueue);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler errorHandler(__FILE__,__LINE__);

  MyEventHandler eventHandler;
  TaskPoolReader<std::string,MyEventHandler> reader(testQueue, timerMock, eventHandler, errorHandler);

  reader();

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 0);

  // clean up to prevent memory leak
  timerMock->Cancel(__FILE__,__LINE__);
  BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 0);
}

BOOST_AUTO_TEST_CASE(TestTaskPoolReaderAdd)
{
    typedef EventQueue<std::string> TestQueue;
    typedef boost::shared_ptr<TestQueue> TestQueuePointer;

    TestQueuePointer testQueue(new TestQueue);
    TimerMockPointer timerMock(new TimerMock);
    AutoCompletionHandler errorHandler(__FILE__,__LINE__);

    MyEventHandler eventHandler;
    TaskPoolReader<std::string,MyEventHandler> reader(testQueue, timerMock, eventHandler, errorHandler);

    reader();

    timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    testQueue->AsyncAdd(std::string("hello"), addHandler);

    BOOST_CHECK_EQUAL(addHandler.Calls(), 1);
    BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 1);
    BOOST_CHECK_EQUAL(eventHandler.GetEvent(), "hello");

    // clean up to prevent memory leak
    timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);
    timerMock->Cancel(__FILE__,__LINE__);

    testQueue.reset();
    timerMock.reset();
}

BOOST_AUTO_TEST_CASE(TestTaskPoolReaderLoop)
{
    typedef EventQueue<std::string> TestQueue;
    typedef boost::shared_ptr<TestQueue> TestQueuePointer;

    TestQueuePointer testQueue(new TestQueue);
    TimerMockPointer timerMock(new TimerMock);
    AutoCompletionHandler errorHandler(__FILE__,__LINE__);

    MyEventHandler eventHandler;
    TaskPoolReader<std::string,MyEventHandler> reader(testQueue, timerMock, eventHandler, errorHandler);

    reader();

    timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    testQueue->AsyncAdd(std::string("hello"), addHandler);
    testQueue->AsyncAdd(std::string("there"), addHandler);

    BOOST_CHECK_EQUAL(eventHandler.GetCallCount(), 2);

    // clean up to prevent memory leak
    timerMock->Cancel(__FILE__,__LINE__);

    testQueue.reset();
    timerMock.reset();
}

BOOST_AUTO_TEST_CASE(TestBadEventHandler1)
{
    typedef EventQueue<std::string> TestQueue;
    typedef boost::shared_ptr<TestQueue> TestQueuePointer;

    TestQueuePointer testQueue(new TestQueue);
    TimerMockPointer timerMock(new TimerMock);
    TestCompletionHandler errorHandler;

    MyBadEventHandler1 eventHandler;
    TaskPoolReader<std::string,MyBadEventHandler1> reader(testQueue, timerMock, eventHandler, errorHandler);

    reader();

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    testQueue->AsyncAdd(std::string("hello"), addHandler);

    BOOST_CHECK(errorHandler.LastError().GetErrorCode() ==
                AsioExpress::ErrorCode::UniqueEventTimeout);

    // clean up to prevent memory leak
    timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestBadEventHandler2)
{
    typedef EventQueue<std::string> TestQueue;
    typedef boost::shared_ptr<TestQueue> TestQueuePointer;

    TestQueuePointer testQueue(new TestQueue);
    TimerMockPointer timerMock(new TimerMock);
    TestCompletionHandler errorHandler;

    MyBadEventHandler2 eventHandler;
    TaskPoolReader<std::string,MyBadEventHandler2> reader(testQueue, timerMock, eventHandler, errorHandler);

    reader();

    AutoCompletionHandler addHandler(__FILE__, __LINE__);
    testQueue->AsyncAdd(std::string("hello"), addHandler);

    BOOST_CHECK(errorHandler.LastError().GetErrorCode() ==
                AsioExpress::Common::ErrorCode::ContractViolation);

    // clean up to prevent memory leak
    timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_SUITE_END()
