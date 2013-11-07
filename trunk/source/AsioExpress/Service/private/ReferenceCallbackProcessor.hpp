//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp> 

#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpressError/CommonException.hpp"
#include "AsioExpress/Service/ServiceCommand.hpp"

namespace AsioExpress {
    namespace Service {

template <typename Handler> 
class ReferenceCallbackProcessor
{ 
public: 
    ReferenceCallbackProcessor(
            boost::asio::io_service &ioService, 
            ServiceCommand & command,
            Handler handler) : 
        ioService(ioService), 
        work(ioService),
        command(command),
        handler(handler) 
    { 
    } 

    void operator()()
    { 
      try
      {
         AsioExpress::Error error = command.Run();

        // call the command completion handler
        ioService.post(boost::asio::detail::bind_handler(handler, error)); 
      }
      catch (AsioExpress::CommonException const & e)
      {
        // call the command completion handler
        ioService.post(boost::asio::detail::bind_handler(handler, e.GetError())); 
      }
      catch (std::exception const & e)
      {
        // call the command completion handler
        ioService.post(boost::asio::detail::bind_handler(
          handler, 
          AsioExpress::Error(
            AsioExpress::Common::ErrorCode::StandardException, e.what()))); 
      }
      catch (...)
      {
        // call the command completion handler
        ioService.post(boost::asio::detail::bind_handler(
          handler, 
          AsioExpress::Error(
            AsioExpress::Common::ErrorCode::UnknownException))); 
      }
    } 

private: 
    boost::asio::io_service &         ioService; 
    boost::asio::io_service::work     work; 
    ServiceCommand &                  command;
    Handler                           handler; 
}; 

} // namespace Service
} // namespace AsioExpress
