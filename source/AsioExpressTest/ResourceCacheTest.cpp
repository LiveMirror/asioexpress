//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Testing/TimerMock.hpp"
#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"

#include "AsioExpress/EventHandling/ResourceCache.hpp"

using namespace AsioExpress;

class MyResourceCache : public AsioExpress::ResourceCache<std::string, int>
{
public:
  MyResourceCache() : m_itemCount(0), m_keyParam(0), m_removeCalls(0), m_updateCalls(0) {}
  void SetError(AsioExpress::Error error) {m_error = error;}
  int GetRemoveCalls() const {return m_removeCalls;}
  int GetUpdateCalls() const {return m_updateCalls;}
  int GetKeyParam() const {return m_keyParam;}
protected:
  virtual void AsyncRemoveItem(
      Key key,
      ResourceItemPointer item, 
      CompletionHandler completionHandler)
  {
    ++m_removeCalls;

    std::ostringstream message;
    message << "Removed Item " << ++m_itemCount;
    *item = message.str();

    m_keyParam = key;
    completionHandler(AsioExpress::Error());
  }

  virtual void AsyncUpdateAllItems(
      CacheUpdatePointer update, 
      CompletionHandler completionHandler)
  {
    ++m_updateCalls;

    update->SetSize(1,0);
    update->SetSize(2,2);
    update->SetSize(3,6);
    update->SetSize(4,8);

    completionHandler(m_error);
  }

  virtual void ErrorHandler(
    AsioExpress::Error /*error*/)
  {
  }

private:
    int                     m_itemCount;
    int                     m_keyParam;
    int                     m_removeCalls;
    int                     m_updateCalls;
    AsioExpress::Error      m_error;
};

BOOST_AUTO_TEST_SUITE(ResourceCacheTest)

BOOST_AUTO_TEST_CASE(TestInitialization)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler completionHandler(__FILE__, __LINE__);

  resourceCache.AsyncUpdate(completionHandler);

  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 0U);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(2), 2U);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(3), 6U);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(4), 8U);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(5), 0U);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);

  MyResourceCache::ResourceItemPointer item(new std::string);
  TimerMockPointer waitTimer(new TimerMock);
  AutoCompletionHandler getHandler(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    2,
    item, 
    waitTimer, 
    getHandler);

  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 2);
  BOOST_CHECK_EQUAL(*item, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(getHandler.Calls(), 1);


  resourceCache.AsyncGet(
    2,
    item, 
    waitTimer, 
    getHandler);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 2);
  BOOST_CHECK_EQUAL(*item, std::string("Removed Item 2"));
  BOOST_CHECK_EQUAL(getHandler.Calls(), 2);
}

BOOST_AUTO_TEST_CASE(TestInitializationError)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;
  resourceCache.SetError(Error(AsioExpress::ErrorCode::EventQueueTimeout));

  TestCompletionHandler completionHandler;

  resourceCache.AsyncUpdate(completionHandler);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError().GetErrorCode() == AsioExpress::ErrorCode::EventQueueTimeout);
}

BOOST_AUTO_TEST_CASE(TestAddOne)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  resourceCache.Add(1, 3);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 3U);
}

BOOST_AUTO_TEST_CASE(TestAddTwo)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  resourceCache.Add(1, 2);
  resourceCache.Add(1, 3);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 5U);
}

BOOST_AUTO_TEST_CASE(TestWaitOne)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item(new std::string);
  TimerMockPointer waitTimer(new TimerMock);
  AutoCompletionHandler getHandler(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    1,
    item, 
    waitTimer, 
    getHandler);

  BOOST_CHECK_EQUAL(getHandler.Calls(), 0);

  resourceCache.Add(1, 1);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 0U);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 1);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 1);
  BOOST_CHECK_EQUAL(*item, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(getHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestWaitOneAddOther)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item(new std::string);
  TimerMockPointer waitTimer(new TimerMock);
  AutoCompletionHandler getHandler(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    1,
    item, 
    waitTimer, 
    getHandler);

  BOOST_CHECK_EQUAL(getHandler.Calls(), 0);

  resourceCache.Add(2, 1);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(2), 3U);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 0);
  BOOST_CHECK_EQUAL(getHandler.Calls(), 0);
}


BOOST_AUTO_TEST_CASE(TestWaitTwoAddOne)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item(new std::string);
  TimerMockPointer waitTimer1(new TimerMock);
  TimerMockPointer waitTimer2(new TimerMock);
  AutoCompletionHandler getHandler1(__FILE__, __LINE__);
  AutoCompletionHandler getHandler2(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    1,
    item, 
    waitTimer1, 
    getHandler1);
  resourceCache.AsyncGet(
    1,
    item, 
    waitTimer2, 
    getHandler2);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 0);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 0);

  resourceCache.Add(1, 1);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 0U);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 1);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 1);
  BOOST_CHECK_EQUAL(*item, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 0);
}

BOOST_AUTO_TEST_CASE(TestWaitTwoAddTwo)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item1(new std::string);
  MyResourceCache::ResourceItemPointer item2(new std::string);
  TimerMockPointer waitTimer1(new TimerMock);
  TimerMockPointer waitTimer2(new TimerMock);
  AutoCompletionHandler getHandler1(__FILE__, __LINE__);
  AutoCompletionHandler getHandler2(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    1,
    item1, 
    waitTimer1, 
    getHandler1);
  resourceCache.AsyncGet(
    1,
    item2, 
    waitTimer2, 
    getHandler2);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 0);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 0);

  resourceCache.Add(1, 2);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 0U);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 2);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 1);
  BOOST_CHECK_EQUAL(*item1, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(*item2, std::string("Removed Item 2"));
  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestWaitTwoAddThree)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item1(new std::string);
  MyResourceCache::ResourceItemPointer item2(new std::string);
  MyResourceCache::ResourceItemPointer item3(new std::string);
  TimerMockPointer waitTimer1(new TimerMock);
  TimerMockPointer waitTimer2(new TimerMock);
  TimerMockPointer waitTimer3(new TimerMock);
  AutoCompletionHandler getHandler1(__FILE__, __LINE__);
  AutoCompletionHandler getHandler2(__FILE__, __LINE__);
  AutoCompletionHandler getHandler3(__FILE__, __LINE__);
  resourceCache.AsyncGet(
    1,
    item1, 
    waitTimer1, 
    getHandler1);
  resourceCache.AsyncGet(
    1,
    item2, 
    waitTimer2, 
    getHandler2);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 0);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 0);

  resourceCache.Add(1, 3);
  BOOST_CHECK_EQUAL(resourceCache.GetCacheSize(1), 1U);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 2);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 1);
  BOOST_CHECK_EQUAL(*item1, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(*item2, std::string("Removed Item 2"));
  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 1);

  resourceCache.AsyncGet(
    1,
    item3, 
    waitTimer3, 
    getHandler3);

  BOOST_CHECK_EQUAL(resourceCache.GetRemoveCalls(), 3);
  BOOST_CHECK_EQUAL(resourceCache.GetKeyParam(), 1);
  BOOST_CHECK_EQUAL(*item1, std::string("Removed Item 1"));
  BOOST_CHECK_EQUAL(*item2, std::string("Removed Item 2"));
  BOOST_CHECK_EQUAL(*item3, std::string("Removed Item 3"));
  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler3.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestShutDown)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item1(new std::string);
  MyResourceCache::ResourceItemPointer item2(new std::string);
  TimerMockPointer waitTimer1(new TimerMock);
  TimerMockPointer waitTimer2(new TimerMock);
  TestCompletionHandler getHandler1;
  TestCompletionHandler getHandler2;
  resourceCache.AsyncGet(
    1,
    item1, 
    waitTimer1, 
    getHandler1);
  resourceCache.AsyncGet(
    1,
    item2, 
    waitTimer2, 
    getHandler2);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 0);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 0);

  waitTimer1->AssertAsyncWaitCalled(__FILE__, __LINE__);
  waitTimer2->AssertAsyncWaitCalled(__FILE__, __LINE__);

  resourceCache.ShutDown();

  waitTimer1->AssertStopCalled(__FILE__, __LINE__);
  waitTimer1->Cancel(__FILE__, __LINE__);

  waitTimer2->AssertStopCalled(__FILE__, __LINE__);
  waitTimer2->Cancel(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK_EQUAL(getHandler2.Calls(), 1);

  BOOST_CHECK(getHandler1.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
  BOOST_CHECK(getHandler2.LastError().GetErrorCode() == 
    boost::asio::error::operation_aborted);
}

BOOST_AUTO_TEST_CASE(TestCacheUpdate)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache::CacheUpdate update1;
  MyResourceCache::CacheUpdate update2;
  MyResourceCache::CacheUpdate update3;

  update1.SetSize(1,2);
  update1.SetSize(2,4);
  update1.SetSize(3,6);

  update2.SetSize(1,2);
  update2.SetSize(2,4);
  update2.SetSize(3,6);

  update3.SetSize(1,2);
  update3.SetSize(2,4);
  update3.SetSize(3,6);
  update3.SetSize(4,8);

  BOOST_CHECK(update1 == update2);
  BOOST_CHECK(update2 == update1);
  BOOST_CHECK(update2 != update3);
}

BOOST_AUTO_TEST_CASE(TestTimeout)
{
  using namespace AsioExpress;
  using namespace AsioExpress::Testing;
  using namespace std;

  MyResourceCache resourceCache;

  AutoCompletionHandler initHandler(__FILE__, __LINE__);
  resourceCache.AsyncUpdate(initHandler);

  MyResourceCache::ResourceItemPointer item(new std::string);
  TimerMockPointer waitTimer1(new TimerMock);
  TestCompletionHandler getHandler1;
  resourceCache.AsyncGet( 
    1,
    item, 
    waitTimer1, 
    getHandler1);

  waitTimer1->AssertAsyncWaitCalled(__FILE__, __LINE__);
  waitTimer1->Expire(__FILE__, __LINE__);

  BOOST_CHECK_EQUAL(getHandler1.Calls(), 1);
  BOOST_CHECK(getHandler1.LastError().GetErrorCode() == 
    AsioExpress::ErrorCode::ResourceCacheTimeout);
}

BOOST_AUTO_TEST_SUITE_END()
