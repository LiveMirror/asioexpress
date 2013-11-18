//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressConfig/config.hpp"

#include <set>
#include <boost/asio.hpp>
#include <iostream>
#include <csignal>

#include "AsioExpress/Platform/Sleep.hpp"
#include "AsioExpress/MessagePort/Ipc/EndPoint.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePort.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePortAcceptor.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/ClientServer/MessagePortServer.hpp"
#include "AsioExpress/ClientServer/MessagePortId.hpp"

typedef std::set<AsioExpress::MessagePort::MessagePortId> ConnectionList;
ConnectionList connectionList;

boost::asio::io_service ioService;
boost::function0<void> shutdown_function;

//BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
//{
//  switch (ctrl_type)
//  {
//  case CTRL_C_EVENT:
//  case CTRL_BREAK_EVENT:
//  case CTRL_CLOSE_EVENT:
//  case CTRL_SHUTDOWN_EVENT:
//    ioService.post(shutdown_function); 
//    return TRUE;
//  default:
//    return FALSE;
//  }
//}

// SIGINT handler
void int_handler(int)
{
  ioService.post(shutdown_function); 
}

typedef AsioExpress::MessagePort::MessagePortServer<AsioExpress::MessagePort::Ipc::MessagePortAcceptor> ServerType;

class MyEventHandler : public AsioExpress::MessagePort::ServerEventHandler
{
public:
  virtual void ClientConnected(
      AsioExpress::MessagePort::ServerConnection connection) 
  {
    std::cout 
      << "Client connected; ID="
      << connection.GetMessagePortId()
      << "\n";
    connectionList.insert(connection.GetMessagePortId());
  }

  virtual void ClientDisconnected(
      AsioExpress::MessagePort::ServerConnection connection, 
      AsioExpress::Error error) 
  {
    std::cout 
      << "Client disconnected; ID=" 
      << connection.GetMessagePortId() 
      << "; "
      << error.Message()
      << "\n";
    connectionList.erase(connection.GetMessagePortId());
  }

  virtual void AsyncProcessMessage(
      AsioExpress::MessagePort::ServerMessage serverMessage)
  {
    serverMessage.AsyncBroadcast(
      serverMessage.GetDataBuffer(),
      serverMessage.GetCompletionHandler());

    //ConnectionList::iterator  id = connectionList.begin();
    //ConnectionList::iterator end = connectionList.end();
    //for(; id != end; ++id)
    //{
    //  serverMessage.server->AsyncSend(
    //    *id, 
    //    serverMessage.dataBuffer,
    //    boost::bind(&MyEventHandler::SendCompleted, this, _1, serverMessage));    
    //}
    //serverMessage.completionHandler(AsioExpress::Error());
  }
  
  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ServerConnection,
      AsioExpress::Error error)
  {
    std::cout
      << "Connection error; "
      << error.Message()
      << "\n";
    return error;
  }

  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ServerMessage, 
      AsioExpress::Error error)
  {
    std::cout
      << "Message error; "
      << error.Message()
      << "\n";
    return error;
  }

private:
  void SendCompleted(
    AsioExpress::Error error,
    AsioExpress::MessagePort::ServerMessage serverMessage)
  {
    std::cout
      << "Send Completed; "
      << error.Message()
      << "\n";
  }
};

int main(int argc, char* argv[])
{
  {
    ServerType server(
      ioService, 
      AsioExpress::MessagePort::Ipc::EndPoint("nowhere"),
      new MyEventHandler);

    // Set console control handler to allow server to be stopped.
    shutdown_function = boost::bind(&ServerType::Stop, &server);
 
    //SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    signal(SIGINT, int_handler);

    server.Start();

    ioService.reset();
    ioService.run();
  }

  std::cout << "\nPress any key to exit...\n";

  return 0;
}
