//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/Yield.hpp"
#include "AsioExpress/Proc/RunProcWithErrors.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

class ExternComponent
{
public:
  void AsyncDoThis(
      int n,
      AsioExpress::CompletionHandler completionHandler)
  {
    if (n==0)
      completionHandler(AsioExpress::Error(AsioExpress::ErrorCode::EventQueueTimeout));
    else
      completionHandler(AsioExpress::Error());
  }

  void AsyncDoThat(
      int n,
      AsioExpress::CompletionHandler completionHandler)
  {
    if (n==0)
      completionHandler(AsioExpress::Error(AsioExpress::ErrorCode::EventQueueTimeout));
    else
      completionHandler(AsioExpress::Error());
  }
};

typedef std::vector<std::string> StringList;
typedef boost::shared_ptr<StringList> StringListPointer;

static void MyProcessor(
  AsioExpress::Error error,
  AsioExpress::Proc::Context context,
  ExternComponent * externComponent,
  int n,
  StringListPointer list)
{
  REENTER(context.Coroutine())
  {
    for (int i=0; i<n; ++i)
      list->push_back(boost::lexical_cast<std::string>(i));

    YIELD
    {
      externComponent->AsyncDoThis(
        n,
        PushProcWithErrors(&MyProcessor,context,externComponent,n,list));
    }
    if (error)
    {
      // ignore
    }      

    YIELD
    {
      externComponent->AsyncDoThat(
        n,
        PushProcWithErrors(&MyProcessor,context,externComponent,n,list));
    }
    if (error)
    {
      context.Complete(error);
      return;
    }

    context.Complete();
  }
}

struct Setup 
{
  Setup() : 
    m_externComponent(new ExternComponent)
  {
  }

  ~Setup()         
  { 
    delete m_externComponent;
    m_externComponent = 0;
  }

  ExternComponent * m_externComponent;
};

BOOST_FIXTURE_TEST_SUITE(RunProcTest, Setup)

BOOST_AUTO_TEST_CASE(TestRunProcWithErrors)
{
  using namespace AsioExpress::Proc;
  using namespace AsioExpress;

  boost::asio::io_service ioService;
  int const N = 5;
  StringListPointer list(new StringList);
  AsioExpress::Testing::AutoCompletionHandler completionHandler(__FILE__,__LINE__);

  RunProcWithErrors(
    &MyProcessor,
    Context(ioService,__FUNCTION__,completionHandler),
    m_externComponent,
    N,
    list);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestRunProcWithErrorsError)
{
  using namespace AsioExpress::Proc;
  using namespace AsioExpress;

  boost::asio::io_service ioService;
  int const N = 0;
  StringListPointer list(new StringList);
  AsioExpress::Testing::TestCompletionHandler completionHandler;

  RunProcWithErrors(
    &MyProcessor,
    Context(ioService,__FUNCTION__,completionHandler),
    m_externComponent,
    N,
    list);

  BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
  BOOST_CHECK(completionHandler.LastError() == AsioExpress::ErrorCode::EventQueueTimeout);
}

BOOST_AUTO_TEST_SUITE_END()
