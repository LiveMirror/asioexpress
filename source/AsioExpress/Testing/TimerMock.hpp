//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp> 
#include <boost/test/unit_test.hpp>

#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpressError/Check.hpp"

#define BOOST_CHECK_EQUAL_LOC(file,line,x,y) BOOST_CHECK_MESSAGE( x == y, #x << " != "  << #y << "; " << x << " != " << y << " file: " << file << ":" << line);

namespace AsioExpress {
namespace Testing {

class TimerMock : public AsioExpress::Timer
{
public:
  TimerMock() :
    m_isWaiting(false),
    m_waitCallCount(0),
    m_stopCallCount(0),
    m_milliseconds(0),
    m_completionHandler(0)
  {
  }

  virtual void AsyncWait(
      CompletionHandler completionHandler)
  {
    BOOST_CHECK_MESSAGE(!m_isWaiting, "TimerMock: Too many AsyncWait calls.");
    m_isWaiting = true;
    ++m_waitCallCount;
    m_completionHandler = completionHandler;
  }

  virtual void AsyncWait(
      unsigned int milliseconds, 
      CompletionHandler completionHandler)
  {
    BOOST_CHECK_MESSAGE(!m_isWaiting, "TimerMock: Too many AsyncWait calls.");
    m_isWaiting = true;
    ++m_waitCallCount;
    m_milliseconds = milliseconds;
    m_completionHandler = completionHandler;
  }

  virtual void Stop()
  {
    ++m_stopCallCount;
    m_isWaiting = false;
    // Note: we cannot call completion handler here as it will violate the
    // timer concurency guarantee.
  }

  void AssertAsyncWaitCalled(char const * const file, int line, unsigned int milliseconds = 0)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_waitCallCount, 1);
    BOOST_CHECK_EQUAL_LOC(file, line, milliseconds, m_milliseconds);
    m_waitCallCount = 0;
  }

  void AssertStopCalled(char const * const file, int line)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_stopCallCount, 1);
    m_stopCallCount = 0;
  }

  void AssertNoAsyncWait(char const * const file, int line)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_waitCallCount, 0);
  }

  void AssertNoStop(char const * const file, int line)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_stopCallCount, 0);
  }

  void Expire(char const * const file, int line)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_completionHandler.empty(), false);
    BOOST_CHECK_EQUAL_LOC(file, line, m_isWaiting, true);
    m_isWaiting = false;
    m_completionHandler(AsioExpress::Error());
    m_completionHandler.clear();
  }

  void Cancel(char const * const file, int line)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_completionHandler.empty(), false);
    m_completionHandler(AsioExpress::Error(boost::asio::error::operation_aborted));
    m_completionHandler.clear();
  }

  void Error(char const * const file, int line, AsioExpress::Error error)
  {
    BOOST_CHECK_EQUAL_LOC(file, line, m_completionHandler.empty(), false);
    m_completionHandler(error);
  }

private:
  bool                  m_isWaiting;
  int                   m_waitCallCount;
  int                   m_stopCallCount;
  unsigned int         m_milliseconds;
  CompletionHandler     m_completionHandler;
};

typedef boost::shared_ptr<TimerMock> TimerMockPointer;

} // namespace Test
} // namespace AsioExpress
