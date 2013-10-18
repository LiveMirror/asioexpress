//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/ClientEventHandler.hpp"

#include "SampleTcpClient1/Logger.hpp"
#include "SampleTcpClient1/Message.hpp"
#include "SampleTcpClient1/AppErrorHandler.hpp"

typedef boost::function<void()> ReconnectHandler;

class EventHandler : public AsioExpress::MessagePort::ClientEventHandler
{
public:
  EventHandler(
      AppErrorHandlerPointer errorHandler,
      std::string const & clientName, 
      const ReconnectHandler& reconnect) :
    m_logger("SampleTcpClient1.EventHandler", 5),
    m_errorHandler(errorHandler),
    m_clientName(clientName),
    m_reconnectHandler(reconnect)
  {
  }

  virtual void ClientConnected(AsioExpress::MessagePort::ClientConnection connection)
  {
    LOG_INFO_STR(connection.GetIoService(), m_logger, "Client connected.");
  }

  virtual void ClientDisconnected(AsioExpress::MessagePort::ClientConnection connection, AsioExpress::Error)
  {
    LOG_INFO_STR(connection.GetIoService(), m_logger, "Client disconnected.");
    m_reconnectHandler();
  }

  virtual void AsyncProcessMessage(AsioExpress::MessagePort::ClientMessage clientMessage)
  {
    CommandMessagePointer message(new Message);

    message->ParseFromArray(clientMessage.GetDataBuffer()->Get(), clientMessage.GetDataBuffer()->Size());

    LOG_INFO(clientMessage.GetIoService(), m_logger, "Received: " <<  message->GetText());

    clientMessage.CallCompletionHandler(AsioExpress::Error());
  }

  virtual AsioExpress::Error ConnectionError(AsioExpress::MessagePort::ClientConnection connection, AsioExpress::Error error)
  {
    LOG_INFO(connection.GetIoService(), m_logger, "Connection Error: " << error.Message());
    return m_errorHandler->ConnectionError(connection, error);
  }

  virtual AsioExpress::Error MessageError(AsioExpress::MessagePort::ClientMessage message, AsioExpress::Error error)
  {
    LOG_INFO(message.GetIoService(), m_logger, "Message Error: " << error.Message());
    return m_errorHandler->MessageError(message, error);
  }

private:
  EventHandler& operator=(const EventHandler&);

private:
  Logger                    m_logger;
  AppErrorHandlerPointer    m_errorHandler;
  std::string               m_clientName;
  ReconnectHandler          m_reconnectHandler;
};
