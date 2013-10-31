//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Logging/Macros.hpp"
#include "AsioExpress/ClientServer/ServerEventHandler.hpp"

#include "SampleTcpServer1/BasicServer.hpp"
#include "SampleTcpServer1/MyIncomingMessage.hpp"
#include "SampleTcpServer1/MyMessageProcessor.hpp"
#include "SampleTcpServer1/Logger.hpp"
#include "SampleTcpServer1/EchoManager.hpp"
#include "SampleTcpServer1/AppErrorHandler.hpp"

class EventHandler : public AsioExpress::MessagePort::ServerEventHandler
{
public:
  EventHandler(
      AppErrorHandlerPointer errorHandler,
      EchoManagerPointer const & echoManager,
      int sampleArgument) :
    m_logger("SampleTcpServer1.EventHandler", 5),
    m_errorHandler(errorHandler),
    m_echoManager(echoManager),
    m_sampleArgument(sampleArgument)
  {
  }

  virtual void ClientConnected(AsioExpress::MessagePort::ServerConnection connection)
  {
    std::ostringstream message;
    message << "Client connected; MessagePortId=" << connection.GetMessagePortId();
    INFO_MSG(connection.GetIoService(), m_logger, message.str());
  }

  virtual void ClientDisconnected(AsioExpress::MessagePort::ServerConnection connection, AsioExpress::Error)
  {
    std::ostringstream message;
    message << "Client disconnected; MessagePortId=" << connection.GetMessagePortId();
    INFO_MSG(connection.GetIoService(), m_logger, message.str());
  }

  virtual void AsyncProcessMessage(AsioExpress::MessagePort::ServerMessage serverMessage)
  {
    INFO_MSG(serverMessage.GetIoService(), m_logger, "Message Recieved.");

    MyIncomingMessage message;
    message.ParseFromArray(serverMessage.GetDataBuffer()->Get(), serverMessage.GetDataBuffer()->Size());

    INFO_MSG(serverMessage.GetIoService(), m_logger, "Message type:" << message.message_type());

    if (message.message_type() == 1)
    {
      m_echoManager->AsyncEcho(serverMessage, m_sampleArgument, serverMessage.GetCompletionHandler());

      // Old method of creating a function object based coroutine.
      //MyMessageProcessor proc(serverMessage, m_sampleArgument);
      //proc();
    }
  }

  virtual AsioExpress::Error ConnectionError(AsioExpress::MessagePort::ServerConnection connection, AsioExpress::Error error)
  {
    return m_errorHandler->ConnectionError(connection, error);
  }

  virtual AsioExpress::Error MessageError(AsioExpress::MessagePort::ServerMessage message, AsioExpress::Error error)
  {
    return m_errorHandler->MessageError(message, error);
  }

private:
  Logger                  m_logger;
  AppErrorHandlerPointer  m_errorHandler;
  EchoManagerPointer      m_echoManager;
  int                     m_sampleArgument;
};
