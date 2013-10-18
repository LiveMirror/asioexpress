//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/Timer/DateTimer.hpp"

namespace AsioExpress {

class StandardDateTimer : public AsioExpress::DateTimer
{
public:
  virtual ~StandardDateTimer() {};

  StandardDateTimer(boost::asio::io_service & ioService) :
    m_timer(ioService)
  {
  }

  virtual void AsyncWait(
      boost::posix_time::ptime time, 
      CompletionHandler completionHandler)
  {
    m_timer.expires_at(time);
    m_timer.async_wait(AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
  }

  virtual void Stop()
  {
    m_timer.cancel();
  }

private:
  boost::asio::deadline_timer m_timer;
};

} // namespace AsioExpress
