//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/ServerConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerMessage
{
public:
  ServerMessage(
      AsioExpress::MessagePort::ServerConnection connection,
      DataBufferPointer dataBuffer,
      AsioExpress::CompletionHandler completionHandler) :
    connection(connection),
    dataBuffer(dataBuffer),
    completionHandler(completionHandler)
  {
  }

  boost::asio::io_service & GetIoService() const;

  AsioExpress::MessagePort::DataBufferPointer GetDataBuffer() const;

  MessagePortId GetMessagePortId() const;

  AsioExpress::MessagePort::ServerConnection GetConnection() const;

  AsioExpress::CompletionHandler GetCompletionHandler() const;

  template<typename H>
  void AsyncSend(
      MessagePortId id,
      DataBufferPointer buffer,
      H completionHandler);

  template<typename H>
  void AsyncBroadcast(
      DataBufferPointer buffer,
      H completionHandler);

  void CallCompletionHandler(AsioExpress::Error const & error);

private:
  ServerMessage & operator=(ServerMessage const &);

  AsioExpress::MessagePort::ServerConnection  connection;
  DataBufferPointer                           dataBuffer;
  AsioExpress::CompletionHandler              completionHandler;
};

typedef boost::shared_ptr<ServerMessage> ServerMessagePointer;

inline boost::asio::io_service &
ServerMessage::GetIoService() const
{
  return connection.GetIoService();
}

inline AsioExpress::MessagePort::DataBufferPointer
ServerMessage::GetDataBuffer() const
{
  return dataBuffer;
}

inline MessagePortId ServerMessage::GetMessagePortId() const
{
  return connection.GetMessagePortId();
}

inline AsioExpress::MessagePort::ServerConnection ServerMessage::GetConnection() const
{
  return connection;
}

inline AsioExpress::CompletionHandler ServerMessage::GetCompletionHandler() const
{
  return completionHandler;
}

template<typename H>
void ServerMessage::AsyncSend(
    MessagePortId id,
    DataBufferPointer buffer,
    H completionHandler)
{
  connection.GetServer()->AsyncSend(id, buffer, completionHandler);
}

template<typename H>
void ServerMessage::AsyncBroadcast(
    DataBufferPointer buffer,
    H completionHandler)
{
  connection.GetServer()->AsyncBroadcast(buffer, completionHandler);
}

inline void ServerMessage::CallCompletionHandler(
    AsioExpress::Error const & error)
{
  AsioExpress::CallCompletionHandler(
    connection.GetIoService(),
    completionHandler,
    error);
  completionHandler = 0;
}

} // namespace MessagePort
} // namespace AsioExpress
