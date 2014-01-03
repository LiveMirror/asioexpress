//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/Timer/TickCountTimer.hpp"

namespace AsioExpress {

class StandardTimer : public AsioExpress::Timer
{
public:
  virtual ~StandardTimer() {};

  StandardTimer(boost::asio::io_service & ioService) :
    m_timer(ioService),
    m_milliseconds(0)
  {
  }

  StandardTimer(
      boost::asio::io_service & ioService, 
      unsigned int milliseconds) :
    m_timer(ioService),
    m_milliseconds(milliseconds)
  {
  }

  virtual void SetWaitTime(
      unsigned int milliseconds)
  {
      m_milliseconds = milliseconds;
  }

  virtual void AsyncWait(
      CompletionHandler completionHandler)
  {
    m_timer.expires_from_now(m_milliseconds);
    m_timer.async_wait(AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
  }

  virtual void AsyncWait(
      unsigned int milliseconds, 
      CompletionHandler completionHandler)
  {
    m_timer.expires_from_now(milliseconds);
    m_timer.async_wait(AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
  }

  virtual void Stop()
  {
    m_timer.cancel();
  }

private:
  TickCountTimer  m_timer;
  unsigned int   m_milliseconds;
};

} // namespace AsioExpress
