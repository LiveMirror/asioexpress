//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/ClientConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientConnectionProcessor
{
public:
  ClientConnectionProcessor(ClientConnection connection) :
    m_connection(connection)
  {
  }

protected:
  ~ClientConnectionProcessor() {};

  boost::asio::io_service & GetIoService() const;

  MessagePortId GetMessagePortId() const;

  ClientInterfacePointer GetMessagePortClient() const;

  void AsyncSend(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

protected:
  ClientConnection   m_connection;
};

inline boost::asio::io_service & 
ClientConnectionProcessor::GetIoService() const
{
  return m_connection.ioService;
}

inline MessagePortId ClientConnectionProcessor::GetMessagePortId() const
{
  return m_connection.messagePortId;
}
  
inline ClientInterfacePointer ClientConnectionProcessor::GetMessagePortClient() const
{
  return m_connection.client;
}

inline void ClientConnectionProcessor::AsyncSend(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_connection.client->AsyncSend(buffer, completionHandler);
}

} // namespace MessagePort
} // namespace AsioExpress
