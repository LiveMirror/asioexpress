//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/EventHandling/UniqueEventHub.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(EventQueueTest)

BOOST_AUTO_TEST_CASE(TestUeWait)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;

  BOOST_CHECK_NO_THROW(
    testEventHub.Add(std::string("hello")));
}

BOOST_AUTO_TEST_CASE(TestUeEarlyAdd)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.Add(std::string("hello"));

  testEventHub.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}

BOOST_AUTO_TEST_CASE(TestUeAdd)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
    l,
    timerMock,
    waitHandler);

  testEventHub.Add(std::string("hello"));

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}

BOOST_AUTO_TEST_CASE(TestUeTimeout)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string, int> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler completionHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(1, eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string, int> TestEventHub;

  TestEventHub testEventHub;

  TestEventHub::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  AutoCompletionHandler waitHandler1(__FILE__, __LINE__);

  TestEventHub::ListenerPointer l1 = testEventHub.NewListener(1, eventData1);

  testEventHub.AsyncWait(
    l1,
    timerMock1,
    waitHandler1);

  TestEventHub::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  AutoCompletionHandler waitHandler2(__FILE__, __LINE__);

  TestEventHub::ListenerPointer l2 = testEventHub.NewListener(2,eventData2);

  testEventHub.AsyncWait(
    l2,
    timerMock2,
    waitHandler2);

  testEventHub.Add(
    1,
    std::string("hello"));

  testEventHub.Add(
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
  typedef UniqueEventHub<std::string, int> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(1, eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);

  testEventHub.Add(std::string("hello"));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "");
}

BOOST_AUTO_TEST_CASE(TestUeWithMultipleListeners)
{
  typedef UniqueEventHub<std::string, int> TestEventHub;

  TestEventHub testEventHub;

  TestEventHub::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;

  TestEventHub::ListenerPointer l1 = testEventHub.NewListener(77, eventData1);

  testEventHub.AsyncWait(
    l1,
    timerMock1,
    waitHandler1);

  TestEventHub::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler waitHandler2;

  BOOST_CHECK_THROW(
    TestEventHub::ListenerPointer l2 = testEventHub.NewListener(77,eventData2),
    AsioExpress::ContractViolationException);

  timerMock1->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeStopTimer)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
    l,
    timerMock,
    waitHandler);

  BOOST_CHECK_THROW(
    testEventHub.AsyncWait(l, timerMock, waitHandler), 
    AsioExpress::ContractViolationException);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeShutDown)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  testEventHub.ShutDown();

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeShutDownAndWait)
{
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  testEventHub.ShutDown(); 

  TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);

  testEventHub.AsyncWait(
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
  typedef UniqueEventHub<std::string> TestEventHub;

  TestEventHub testEventHub;
  TestEventHub::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  // limited scope of listener
  {
    TestEventHub::ListenerPointer l = testEventHub.NewListener(eventData);
  
    testEventHub.AsyncWait(
      l,
      timerMock,
      waitHandler);

    l.reset();
  }

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_NO_THROW(
    testEventHub.Add(std::string("hello")));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_SUITE_END()
