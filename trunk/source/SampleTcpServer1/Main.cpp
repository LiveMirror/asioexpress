//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include <csignal>

#include "SampleTcpServer1/App.hpp"

boost::function0<void> console_ctrl_function;

// Windows signal handler
//BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
//{
//  switch (ctrl_type)
//  {
//  case CTRL_C_EVENT:
//  case CTRL_BREAK_EVENT:
//  case CTRL_CLOSE_EVENT:
//  case CTRL_SHUTDOWN_EVENT:
//    console_ctrl_function();
//    return TRUE;
//  default:
//    return FALSE;
//  }
//}

// SIGINT handler
void int_handler(int)
{
    console_ctrl_function();
}

class ShutDownFunc
{
public:
  ShutDownFunc(
      boost::asio::io_service & ioService,
      App & app) :
    ioService(ioService),
    app(app)
  {
  }

  void operator()()
  {
    // The App::Stop is posted to the io_service to make sure it is 
    // it is performed in the correct thread.
    ioService.post(
       boost::bind(
          &App::Stop, 
          &app));
  }

private:
  boost::asio::io_service &   ioService;
  App &                       app;
};

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: SampleServer1 <ip> <port>" << std::endl
              << "Example: SampleServer1 127.0.0.1 18000";
    return 1;
  }

  std::string serverIpAddress = argv[1];
  std::string serverPort      = argv[2];

  boost::asio::io_service ioService;

  App app(ioService, serverIpAddress, serverPort);

  // Set console control handler to allow server to be stopped.
  console_ctrl_function = ShutDownFunc(ioService, app);

  // Setup Windows signal handler
  //  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

  signal(SIGINT, int_handler);

  app.Start();

  ioService.reset();
  ioService.run();

  return 0;
}
