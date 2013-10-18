//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/ServerConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerConnectionProcessor
{
public:
  ServerConnectionProcessor(ServerConnection connection) :
    m_connection(connection)
  {
  }

protected:
  ~ServerConnectionProcessor() {};

  boost::asio::io_service & GetIoService() const;

  MessagePortId GetMessagePortId() const;

  void AsyncSend(
      MessagePortId id, 
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

  void AsyncBroadcast(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

private:
  ServerConnection   m_connection;
};

inline boost::asio::io_service & 
ServerConnectionProcessor::GetIoService() const
{
  return m_connection.ioService;
}

inline MessagePortId ServerConnectionProcessor::GetMessagePortId() const
{
  return m_connection.messagePortId;
}

inline void ServerConnectionProcessor::AsyncSend(
    MessagePortId id, 
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_connection.server->AsyncSend(id, buffer, completionHandler);
}

inline void ServerConnectionProcessor::AsyncBroadcast(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_connection.server->AsyncBroadcast(buffer, completionHandler);
}

} // namespace MessagePort
} // namespace AsioExpress
