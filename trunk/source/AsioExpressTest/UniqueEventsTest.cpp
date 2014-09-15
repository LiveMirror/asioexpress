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

BOOST_AUTO_TEST_CASE(TestWait)
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

BOOST_AUTO_TEST_CASE(TestNoListener)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;

  BOOST_CHECK_NO_THROW(
    testEvents.Add(std::string("hello")));
}

BOOST_AUTO_TEST_CASE(TestEarlyAdd)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvents::Listener listener(testEvents);

  listener.New();
  
  testEvents.Add(std::string("hello"));

  listener.AsyncWait(
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(listener.GetEventValue(), "hello");
}

BOOST_AUTO_TEST_CASE(TestAdd)
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

BOOST_AUTO_TEST_CASE(TestAddListenerCopy)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  AutoCompletionHandler waitHandler(__FILE__, __LINE__);

  TestEvents::Listener* listener1(new TestEvents::Listener(testEvents));
  listener1->New();

  TestEvents::Listener* listener2(new TestEvents::Listener(*listener1));  
  delete listener1;

  listener2->AsyncWait(
    timerMock,
    waitHandler);

  testEvents.Add(std::string("hello"));

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(listener2->GetEventValue(), "hello");

  delete listener2;
}

BOOST_AUTO_TEST_CASE(TestTimeout)
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

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
              AsioExpress::ErrorCode::UniqueEventTimeout);
}

BOOST_AUTO_TEST_CASE(TestCancel)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::Listener listener(testEvents);

  listener.New(1);
  
  listener.AsyncWait(
    timerMock,
    waitHandler);
  
  listener.Cancel();
  
  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
              boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestWaitWithKey)
{
  typedef UniqueEvents<std::string, int> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler completionHandler;

  TestEvents::Listener listener(testEvents);

  listener.New();
  
  listener.AsyncWait(
    timerMock,
    completionHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

  listener.Cancel();

  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == 
                boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestAddsWithKey)
{
  typedef UniqueEvents<std::string, int> TestEvents;

  TestEvents testEvents;

  TimerMockPointer timerMock1(new TimerMock);
  AutoCompletionHandler waitHandler1(__FILE__, __LINE__);

  TestEvents::Listener listener1(testEvents);
  
  listener1.New(1);

  listener1.AsyncWait(
    timerMock1,
    waitHandler1);

  TimerMockPointer timerMock2(new TimerMock);
  AutoCompletionHandler waitHandler2(__FILE__, __LINE__);

  TestEvents::Listener listener2(testEvents);
  
  listener2.New(2);

  listener2.AsyncWait(
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
  BOOST_CHECK_EQUAL(listener1.GetEventValue(), "hello");
  BOOST_CHECK_EQUAL(listener2.GetEventValue(), "there");
}

BOOST_AUTO_TEST_CASE(TestTimeoutWithKey)
{
  typedef UniqueEvents<std::string, int> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  TestEvents::Listener listener(testEvents);
  
  listener.New(1);

  listener.AsyncWait(
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);
}

BOOST_AUTO_TEST_CASE(TestTimeoutWithLateAdd)
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

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::UniqueEventTimeout);

  testEvents.Add(std::string("hello"));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(listener.GetEventValue(), "");
}

BOOST_AUTO_TEST_CASE(TestWithMultipleListeners)
{
  typedef UniqueEvents<std::string, int> TestEvents;

  TestEvents testEvents;

  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;

  TestEvents::Listener listener1(testEvents);
  
  listener1.New(77);

  listener1.AsyncWait(
    timerMock1,
    waitHandler1);

  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler waitHandler2;

  TestEvents::Listener listener2(testEvents);
  
  BOOST_CHECK_THROW(
        listener2.New(77),
        AsioExpress::ContractViolationException);

  timerMock1->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestStopTimer)
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

  timerMock->Stop();
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestConcurrentWaits)
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

  BOOST_CHECK_THROW(
    listener.AsyncWait(timerMock, waitHandler), 
    AsioExpress::ContractViolationException);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestShutDown)
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

  testEvents.ShutDown();

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestShutDownAndWait)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  testEvents.ShutDown(); 

  TestEvents::Listener listener(testEvents);

  listener.New();
  
  listener.AsyncWait(
    timerMock,
    waitHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);
  timerMock->AssertNoStop(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestListenerScope)
{
  typedef UniqueEvents<std::string> TestEvents;

  TestEvents testEvents;
  TimerMockPointer timerMock(new TimerMock);
  TestCompletionHandler waitHandler;

  // limited scope of listener
  {
    TestEvents::Listener listener(testEvents);
  
    listener.New();
    
    listener.AsyncWait(
      timerMock,
      waitHandler);

    listener.Cancel();
  }

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_NO_THROW(
    testEvents.Add(std::string("hello")));

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
              boost::asio::error::operation_aborted);

  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_SUITE_END()
