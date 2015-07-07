//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp> 

#include "AsioExpressError/Check.hpp"
#include "AsioExpress/Service/ServiceCommand.hpp"

namespace AsioExpress {
    namespace Service {

class ReferenceProcessor
{ 
public: 
    ReferenceProcessor(
            boost::asio::io_service &ioService, 
            ServiceCommand & command) : 
        work(ioService),
        command(command)
    { 
    } 

    void operator()()
    { 
      AsioExpress::Error error = command.Run();
      CHECK(!error);
    } 

private: 
    boost::asio::io_service::work     work; 
    ServiceCommand &                  command;
}; 

} // namespace Service
} // namespace AsioExpress
