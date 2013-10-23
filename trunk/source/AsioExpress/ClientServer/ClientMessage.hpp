//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/ClientConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientMessage
{
public:
  ClientMessage(
      AsioExpress::MessagePort::ClientConnection connection,
      AsioExpress::MessagePort::DataBufferPointer dataBuffer, 
      AsioExpress::CompletionHandler completionHandler) :
    connection(connection),
    dataBuffer(dataBuffer),
    completionHandler(completionHandler)
  {
  }

  boost::asio::io_service & GetIoService() const;

  AsioExpress::MessagePort::DataBufferPointer GetDataBuffer() const;

  MessagePortId GetMessagePortId() const;

  ClientInterfacePointer GetClient() const;

  AsioExpress::MessagePort::ClientConnection GetConnection() const;

  AsioExpress::CompletionHandler GetCompletionHandler() const;

  template<typename H>
  void AsyncSend(
      DataBufferPointer buffer, 
      H completionHandler);

  void CallCompletionHandler(AsioExpress::Error const & error);

private:
  ClientMessage & operator=(ClientMessage const &);

  AsioExpress::MessagePort::ClientConnection  connection;
  AsioExpress::MessagePort::DataBufferPointer dataBuffer;
  AsioExpress::CompletionHandler              completionHandler;
};

typedef boost::shared_ptr<ClientMessage> ClientMessagePointer;

inline boost::asio::io_service & 
ClientMessage::GetIoService() const
{
  return connection.GetIoService();
}

inline AsioExpress::MessagePort::DataBufferPointer 
ClientMessage::GetDataBuffer() const
{
  return dataBuffer;
}

inline MessagePortId ClientMessage::GetMessagePortId() const
{
  return connection.GetMessagePortId();
}
  
inline ClientInterfacePointer ClientMessage::GetClient() const
{
  return connection.GetClient();
}

inline AsioExpress::MessagePort::ClientConnection ClientMessage::GetConnection() const
{
  return connection;
}

inline AsioExpress::CompletionHandler ClientMessage::GetCompletionHandler() const
{
  return completionHandler;
}

template<typename H>
void ClientMessage::AsyncSend(
    DataBufferPointer buffer, 
    H completionHandler)
{
  connection.GetClient()->AsyncSend(buffer, completionHandler);
}

inline void ClientMessage::CallCompletionHandler(
    AsioExpress::Error const & error)
{
  AsioExpress::CallCompletionHandler(
    connection.GetIoService(), 
    completionHandler, 
    error);
}

} // namespace MessagePort
} // namespace AsioExpress
