//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"

#include "AsioExpress/EventHandling/UniqueEvents.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(UniqueEventsTest)

BOOST_AUTO_TEST_CASE(TestUeWait)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::Listener listener(testEvents);

  listener.New();
  
  listener.AsyncWait(
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 0);

  listener.Cancel();

  // clean up timers to prevent memory leaks
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
                boost::asio::error::operation_aborted);
}

/*
BOOST_AUTO_TEST_CASE(TestUeNoListener)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;

  BOOST_CHECK_NO_THROW(
    testEvents.Add(std::string("hello")));
}

BOOST_AUTO_TEST_CASE(TestUeEarlyAdd)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.Add(std::string("hello"));

  testEvents.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}
*/

BOOST_AUTO_TEST_CASE(TestUeAdd)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvents::Listener listener(testEvents);
                    
  listener.New();
  
  listener.AsyncWait(
    timerMock,
    waitHandler);

  testEvents.Add(std::string("hello"));

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(listener.GetEventValue(), "hello");
}

/*
BOOST_AUTO_TEST_CASE(TestUeTimeout)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
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
  typedef UniqueEvent<std::string, int> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler completionHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(1, eventData);

  testEvents.AsyncWait(
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
*/

BOOST_AUTO_TEST_CASE(TestUeAddsWithKey)
{
  typedef UniqueEvents<std::string, int> TestEvents;

  TestEvents testEvents;

  TimerMockPointer timerMock1(new TimerMock);
  AutoCompletionHandler waitHandler1(__FILE__, __LINE__);

  TestEvents::Listener l1(testEvents);
  
  l1.New(1);

  l1.AsyncWait(
    timerMock1,
    waitHandler1);

  TimerMockPointer timerMock2(new TimerMock);
  AutoCompletionHandler waitHandler2(__FILE__, __LINE__);

  TestEvents::Listener l2(testEvents);
  
  l2.New(2);

  l2.AsyncWait(
    timerMock2,
    waitHandler2);

  testEvents.Add(
    1,
    std::string("hello"));

  testEvents.Add(
    2,
    std::string("there"));

  timerMock1->Cancel(__FILE__,__LINE__);
  timerMock2->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler2.Calls(), 1);
  BOOST_CHECK_EQUAL(l1.GetEventValue(), "hello");
  BOOST_CHECK_EQUAL(l2.GetEventValue(), "there");
}

/*
BOOST_AUTO_TEST_CASE(TestUeTimeoutWithKey)
{
  typedef UniqueEvent<std::string, int> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(1, eventData);

  testEvents.AsyncWait(
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
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);

  testEvents.Add(std::string("hello"));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "");
}

BOOST_AUTO_TEST_CASE(TestUeWithMultipleListeners)
{
  typedef UniqueEvent<std::string, int> TestEvents;

  TestEvents testEvents;

  TestEvents::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;

  TestEvents::ListenerPointer l1 = testEvents.NewListener(77, eventData1);

  testEvents.AsyncWait(
    l1,
    timerMock1,
    waitHandler1);

  TestEvents::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler waitHandler2;

  BOOST_CHECK_THROW(
    TestEvents::ListenerPointer l2 = testEvents.NewListener(77,eventData2),
    AsioExpress::ContractViolationException);

  timerMock1->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeStopTimer)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
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
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
    l,
    timerMock,
    waitHandler);

  BOOST_CHECK_THROW(
    testEvents.AsyncWait(l, timerMock, waitHandler), 
    AsioExpress::ContractViolationException);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestUeShutDown)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
    l,
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  testEvents.ShutDown();

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestUeShutDownAndWait)
{
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  testEvents.ShutDown(); 

  TestEvents::ListenerPointer l = testEvents.NewListener(eventData);

  testEvents.AsyncWait(
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
  typedef UniqueEvent<std::string> TestEvents;

  TestEvents testEvents;
  TestEvents::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  // limited scope of listener
  {
    TestEvents::ListenerPointer l = testEvents.NewListener(eventData);
  
    testEvents.AsyncWait(
      l,
      timerMock,
      waitHandler);

    l.reset();
  }

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_NO_THROW(
    testEvents.Add(std::string("hello")));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);

  timerMock->Cancel(__FILE__,__LINE__);
}
*/
BOOST_AUTO_TEST_SUITE_END()
