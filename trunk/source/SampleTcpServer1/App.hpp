//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"
#include "AsioExpress/Logging/LoggingService.hpp"

#include "SampleTcpServer1/AppErrorHandlerImpl.hpp"
#include "SampleTcpServer1/EventHandler.hpp"
#include "SampleTcpServer1/BasicServer.hpp"
#include "SampleTcpServer1/EchoManager.hpp"

class App
{
public:
  App(
      boost::asio::io_service & ioService,
      std::string const & serverIpAddress,
      std::string const & serverPort) :
    ioService(ioService),
    errorHandler(new AppErrorHandlerImpl),
    echoManager(new EchoManager),
    server(
      ioService, 
      BasicServer::EndPointType(serverIpAddress, serverPort), 
      new EventHandler(errorHandler, echoManager, 42))
  {
    SET_DEBUG_TIMER_PRINT_THRESHOLD(10);
  }

  void Start()
  {
    server.Start();
  }

  void Stop()
  {
    server.Stop();
  }

private:
  boost::asio::io_service &   ioService;
  AppErrorHandlerPointer      errorHandler;
  EchoManagerPointer          echoManager;
  BasicServer                 server;
};
