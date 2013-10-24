//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/Timer/RepeatingTickCountTimer.hpp"

namespace AsioExpress {

class StandardRepeatingTimer : public AsioExpress::Timer
{
public:
  virtual ~StandardRepeatingTimer() {};

  StandardRepeatingTimer(boost::asio::io_service & ioService) :
    m_timer(ioService),
    m_milliseconds(5000)
  {
  }

  StandardRepeatingTimer(
      boost::asio::io_service & ioService, 
      unsigned int milliseconds) :
    m_timer(ioService),
    m_milliseconds(milliseconds)
  {
  }

  virtual void AsyncWait(
      CompletionHandler completionHandler)
  {
    m_timer.Start(
      m_milliseconds,
      AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
  }

  virtual void AsyncWait(
      unsigned int milliseconds, 
      CompletionHandler completionHandler)
  {
    m_timer.Start(
      milliseconds,
      AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
  }

  virtual void Stop()
  {
    m_timer.Stop();
  }

private:
  RepeatingTickCountTimer   m_timer;
  unsigned int             m_milliseconds;
};

} // namespace AsioExpress
