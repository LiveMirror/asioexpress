//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/EventHandling/EventQueue.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(EventQueueTest)

BOOST_AUTO_TEST_CASE(TestEventQueueWait)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);
 
  TestCompletionHandler completionHandler;
  
  testQueue.AsyncWait(
    eventData, 
    timerMock,
    completionHandler);
   
  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

  // clean up to prevent memory link
  timerMock->Cancel(__FILE__,__LINE__);
  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestEventQueueAdd)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  AutoCompletionHandler waitHandler(__FILE__, __LINE__);
  testQueue.AsyncWait(
    eventData, 
    timerMock,
    waitHandler);

  AutoCompletionHandler addHandler(__FILE__, __LINE__);
  testQueue.AsyncAdd(std::string("hello"), addHandler);

  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__,__LINE__);

  BOOST_CHECK_EQUAL(addHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData, "hello");
}

BOOST_AUTO_TEST_CASE(TestEventQueueTimeout)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler waitHandler;
  testQueue.AsyncWait(
    eventData, 
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::EventQueueTimeout);
}

BOOST_AUTO_TEST_CASE(TestEventQueueWaitWithKey)
{
  typedef EventQueue<std::string, int> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler completionHandler;

  testQueue.AsyncWait(
    1,
    eventData, 
    timerMock,
    completionHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

  // clean up to prevent memory link
  timerMock->Cancel(__FILE__,__LINE__);
  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestEventQueueAddsWithKey)
{
  typedef EventQueue<std::string, int> TestQueue;

  TestQueue testQueue;

  TestQueue::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;
  testQueue.AsyncWait(
    1,
    eventData1, 
    timerMock1,
    waitHandler1);

  TestQueue::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  AutoCompletionHandler waitHandler2(__FILE__, __LINE__);
  testQueue.AsyncWait(
    2,
    eventData2, 
    timerMock2,
    waitHandler2);

  AutoCompletionHandler addHandler1(__FILE__, __LINE__);
  testQueue.AsyncAdd(
    1,
    std::string("hello"),
    addHandler1);

  AutoCompletionHandler addHandler2(__FILE__, __LINE__);
  testQueue.AsyncAdd(
    2,
    std::string("there"),
    addHandler2);

  BOOST_CHECK_EQUAL(addHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(addHandler2.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler2.Calls(), 1);
  BOOST_CHECK_EQUAL(*eventData1, "hello");
  BOOST_CHECK_EQUAL(*eventData2, "there");

  // clean up timers to prevent memory leaks
  timerMock1->AssertStopCalled(__FILE__, __LINE__);
  timerMock1->Cancel(__FILE__,__LINE__);
  timerMock2->AssertStopCalled(__FILE__, __LINE__);
  timerMock2->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestEventQueueTimeoutWithKey)
{
  typedef EventQueue<std::string, int> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler waitHandler;
  testQueue.AsyncWait(
    1,
    eventData, 
    timerMock,
    waitHandler);

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);

  timerMock->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler.Calls(), 1);
  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
            AsioExpress::ErrorCode::EventQueueTimeout);
} 

BOOST_AUTO_TEST_CASE(TestStopTimer)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler waitHandler;
  testQueue.AsyncWait(
    eventData, 
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
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler waitHandler;
  testQueue.AsyncWait(
    eventData, 
    timerMock,
    waitHandler);

  BOOST_CHECK_THROW(
    testQueue.AsyncWait(eventData, timerMock, waitHandler), 
    AsioExpress::ContractViolationException);

  // clean up timers to prevent memory leaks
  timerMock->Cancel(__FILE__,__LINE__);
}

BOOST_AUTO_TEST_CASE(TestShutDown)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestCompletionHandler completionHandler;

  testQueue.AsyncWait(
    eventData, 
    timerMock,
    completionHandler);

  testQueue.ShutDown();

  timerMock->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock->AssertStopCalled(__FILE__, __LINE__);
  timerMock->Cancel(__FILE__, __LINE__);

  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestShutDownMultiple)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;

  TestQueue::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler completionHandler1;

  testQueue.AsyncWait(
    eventData1, 
    timerMock1,
    completionHandler1);

  TestQueue::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler completionHandler2;

  testQueue.AsyncWait(
    eventData2, 
    timerMock2,
    completionHandler2);

  testQueue.ShutDown();	

  timerMock1->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock1->AssertStopCalled(__FILE__, __LINE__);
  timerMock1->Cancel(__FILE__, __LINE__);

  timerMock2->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock2->AssertStopCalled(__FILE__, __LINE__);
  timerMock2->Cancel(__FILE__, __LINE__);

  BOOST_CHECK(completionHandler1.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);

  BOOST_CHECK(completionHandler2.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestShutDownAndWait)
{
  typedef EventQueue<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  testQueue.ShutDown();	

  TestCompletionHandler waitHandler;
  testQueue.AsyncWait(
    eventData, 
    timerMock,
    waitHandler);

  TestCompletionHandler addHandler;
  testQueue.AsyncAdd(
    std::string("hello"),
    addHandler);

  timerMock->AssertNoAsyncWait(__FILE__, __LINE__);
  timerMock->AssertNoStop(__FILE__, __LINE__);

  BOOST_CHECK(waitHandler.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);

  BOOST_CHECK(addHandler.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestShutDownWithKey)
{
  typedef EventQueue<std::string, int> TestQueue;

  TestQueue testQueue;

  TestQueue::EventPointer eventData1(new std::string);
  TimerMockPointer timerMock1(new TimerMock);
  TestCompletionHandler waitHandler1;
  testQueue.AsyncWait(
    1,
    eventData1, 
    timerMock1,
    waitHandler1);

  TestQueue::EventPointer eventData2(new std::string);
  TimerMockPointer timerMock2(new TimerMock);
  TestCompletionHandler waitHandler2;
  testQueue.AsyncWait(
    2,
    eventData2, 
    timerMock2,
    waitHandler2);

  testQueue.ShutDown();

  timerMock1->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock1->AssertStopCalled(__FILE__, __LINE__);
  timerMock1->Cancel(__FILE__, __LINE__);

  timerMock2->AssertAsyncWaitCalled(__FILE__, __LINE__);
  timerMock2->AssertStopCalled(__FILE__, __LINE__);
  timerMock2->Cancel(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(waitHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(waitHandler2.Calls(), 1);

  BOOST_CHECK(waitHandler1.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);

  BOOST_CHECK(waitHandler2.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_SUITE_END()
