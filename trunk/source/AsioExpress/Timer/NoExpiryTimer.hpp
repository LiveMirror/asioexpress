//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/Timer/TickCountTimer.hpp"

namespace AsioExpress {

class NoExpiryTimer : public AsioExpress::Timer
{
public:
  NoExpiryTimer(
      boost::asio::io_service & ioService) :
    m_ioService(ioService)
  {      
  }
    
  virtual ~NoExpiryTimer() 
  {
      Stop();
  };

  virtual void AsyncWait(
      CompletionHandler completionHandler)
  {
      m_completionHandler = completionHandler;
  }

  virtual void AsyncWait(
      unsigned int, 
      CompletionHandler)
  {
      CHECK_FAILED("Method not implemented.");
  }

  virtual void SetWaitTime(
      unsigned int)
  {
      CHECK_FAILED("Method not implemented.");
  }

  virtual void Stop()
  {
      if (m_completionHandler)
      {
          AsioExpress::CompletionHandler handler(m_completionHandler); 

          // we need to reset the completion handler so it does not linger.
          m_completionHandler = 0;
          
          AsioExpress::CallCompletionHandler(
              m_ioService, 
              handler, 
              Error(boost::asio::error::operation_aborted));
      }
  }

private:
    boost::asio::io_service &   m_ioService;
    CompletionHandler           m_completionHandler;
};

} // namespace AsioExpress
