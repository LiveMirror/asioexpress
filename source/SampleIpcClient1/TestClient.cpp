//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>

#include "AsioExpress/Error.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"

#include "AsioExpress/MessagePort/Ipc/EndPoint.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePort.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePortAcceptor.hpp"
#include "AsioExpress/Timer/StandardRepeatingTimer.hpp"

#include "AsioExpress/ClientServer/MessagePortClient.hpp"

boost::asio::io_service ioService;
boost::function0<void> shutdown_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    ioService.post(shutdown_function); 
    return TRUE;
  default:
    return FALSE;
  }
}

typedef AsioExpress::MessagePort::MessagePortClient<AsioExpress::MessagePort::Ipc::MessagePort> ClientType;

class MyEventHandler : public AsioExpress::MessagePort::ClientEventHandler
{
public:
  MyEventHandler(boost::asio::io_service & ioService) :
    m_timer(ioService, 2000),
    m_sendCount(0)
  {
  }

  virtual void ClientConnected(
      AsioExpress::MessagePort::ClientConnection connection) 
  {
    printf("Client connected; ID=%d\n", connection.messagePortId);
 
    m_connection.reset(new AsioExpress::MessagePort::ClientConnection(
      connection.ioService, 
      connection.messagePortId, 
      connection.client));

    m_timer.AsyncWait(boost::bind(&MyEventHandler::DoSend, this, _1));
  }

  virtual void ClientDisconnected(
      AsioExpress::MessagePort::ClientConnection connection, 
      AsioExpress::Error) 
  {
    printf("Client disconnected; ID=%d\n", connection.messagePortId);
    m_timer.Stop();
  }

  virtual void AsyncProcessMessage(
      AsioExpress::MessagePort::ClientMessage clientMessage)
  {
    std::string msg;
    msg.assign(clientMessage.dataBuffer->Get(), clientMessage.dataBuffer->Size());
    printf("RECV: %s\n", msg.c_str());
    clientMessage.completionHandler(AsioExpress::Error());
  }
  
  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ClientConnection,
      AsioExpress::Error error)
  {
    printf("Connection error; %s\n", error.Message());
    return error;
  }

  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ClientMessage, 
      AsioExpress::Error error)
  {
    printf("Message error; %s\n", error.Message());
    return error;
  }

private:
  void DoSend(AsioExpress::Error error)
  {
    if (error)
    {
      std::cout
        << "Timer error; "
        << error.Message()
        << "\n";

      m_sendCount = 0;
      return;
    }

    if ( m_sendCount > 9 )
    {
      m_connection->client->Disconnect();
      m_timer.Stop();
      return;
    }

    std::ostringstream message;
    message 
      << "Message " 
      << ++m_sendCount;

    AsioExpress::MessagePort::DataBufferPointer dataBuffer(
      new AsioExpress::MessagePort::DataBuffer(message.str().size()));
    memcpy(dataBuffer->Get(), message.str().c_str(), message.str().size());

    printf("Start send; %s\n", message.str().c_str());

    m_connection->client->AsyncSend(
      dataBuffer, 
      boost::bind(&MyEventHandler::SendCompleted, this, _1));
  }

  void SendCompleted(
    AsioExpress::Error error)
  {
    printf("Send Completed; %s\n", error.Message());
  }

  typedef boost::shared_ptr<AsioExpress::MessagePort::ClientConnection> ClientConnectionPointer; 

  AsioExpress::StandardRepeatingTimer    m_timer;
  int                                     m_sendCount;
  ClientConnectionPointer                 m_connection;
};


int _tmain(int argc, _TCHAR* argv[])
{
  ClientType client(
    ioService, 
    AsioExpress::MessagePort::Ipc::EndPoint("nowhere"),
    new MyEventHandler(ioService));
  client.Connect();

  // Set console control handler to allow client to be stopped.
  shutdown_function = boost::bind(&ClientType::Disconnect, &client);

  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

  ioService.run();

	return 0;
}
