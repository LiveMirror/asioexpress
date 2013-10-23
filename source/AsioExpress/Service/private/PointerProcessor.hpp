//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <boost/asio.hpp> 

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/Service/ServiceCommand.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"

namespace AsioExpress {
    namespace Service {

class PointerProcessor
{ 
public: 
    PointerProcessor(
            boost::asio::io_service &ioService, 
            ServiceCommandPointer command) : 
        ioService(ioService), 
        work(ioService),
        command(command)
    { 
    } 

    void operator()()
    { 
      try
      {
        AsioExpress::Error error = command->Run();
        assert(!error);
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO(DebugMessage(error.Message()))        
    } 

private: 
    boost::asio::io_service &         ioService; 
    boost::asio::io_service::work     work; 
    ServiceCommandPointer             command;
}; 

} // namespace Service
} // namespace AsioExpress
