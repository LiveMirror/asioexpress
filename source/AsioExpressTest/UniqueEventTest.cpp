//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/EventHandling/UniqueEvent.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(EventQueueTest)

BOOST_AUTO_TEST_CASE(TestUeWait)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 0);

  l.reset();

  // clean up timers to prevent memory leaks
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
                boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeNoListener)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;

  BOOST_CHECK_NO_THROW(
    testEvent.Add(std::string("hello")));
}

BOOST_AUTO_TEST_CASE(TestUeEarlyAdd)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.Add(std::string("hello"));

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}

BOOST_AUTO_TEST_CASE(TestUeAdd)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  testEvent.Add(std::string("hello"));

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}

BOOST_AUTO_TEST_CASE(TestUeTimeout)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);
}

BOOST_AUTO_TEST_CASE(TestUeWaitWithKey)
{
  typedef UniqueEvent<std::string, int> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler completionHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(1, eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    completionHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

  l.reset();

  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == 
                boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeAddsWithKey)
{
  typedef UniqueEvent<std::string, int> TestEvent;

  TestEvent testEvent;

  TestEvent::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  AutoCompletionHandler waitHandler1(__FILE__, __LINE__);

  TestEvent::ListenerPointer l1 = testEvent.NewListener(1, eventData1);

  testEvent.AsyncWait(
    l1,
    timerMock1,
    waitHandler1);

  TestEvent::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  AutoCompletionHandler waitHandler2(__FILE__, __LINE__);

  TestEvent::ListenerPointer l2 = testEvent.NewListener(2,eventData2);

  testEvent.AsyncWait(
    l2,
    timerMock2,
    waitHandler2);

  testEvent.Add(
    1,
    std::string("hello"));

  testEvent.Add(
    2,
    std::string("there"));

  timerMock1->Cancel(__FILE__,__LINE__);
  timerMock2->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler2.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData1, "hello");
  BOOST_CHECK_EQUAL(*eventData2, "there");
}

BOOST_AUTO_TEST_CASE(TestUeTimeoutWithKey)
{
  typedef UniqueEvent<std::string, int> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(1, eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);
}

BOOST_AUTO_TEST_CASE(TestUeTimeoutWithLateAdd)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);

  testEvent.Add(std::string("hello"));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "");
}

BOOST_AUTO_TEST_CASE(TestUeWithMultipleListeners)
{
  typedef UniqueEvent<std::string, int> TestEvent;

  TestEvent testEvent;

  TestEvent::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;

  TestEvent::ListenerPointer l1 = testEvent.NewListener(77, eventData1);

  testEvent.AsyncWait(
    l1,
    timerMock1,
    waitHandler1);

  TestEvent::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler waitHandler2;

  BOOST_CHECK_THROW(
    TestEvent::ListenerPointer l2 = testEvent.NewListener(77,eventData2),
    AsioExpress::ContractViolationException);

  timerMock1->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeStopTimer)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Stop();
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeConcurrentWaits)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  BOOST_CHECK_THROW(
    testEvent.AsyncWait(l, timerMock, waitHandler), 
    AsioExpress::ContractViolationException);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeShutDown)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  testEvent.ShutDown();

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeShutDownAndWait)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  testEvent.ShutDown(); 

  TestEvent::ListenerPointer l = testEvent.NewListener(eventData);

  testEvent.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);
  timerMock->AssertNoStop(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeListenerScope)
{
  typedef UniqueEvent<std::string> TestEvent;

  TestEvent testEvent;
  TestEvent::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  // limited scope of listener
  {
    TestEvent::ListenerPointer l = testEvent.NewListener(eventData);
  
    testEvent.AsyncWait(
      l,
      timerMock,
      waitHandler);

    l.reset();
  }

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_NO_THROW(
    testEvent.Add(std::string("hello")));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_SUITE_END()
