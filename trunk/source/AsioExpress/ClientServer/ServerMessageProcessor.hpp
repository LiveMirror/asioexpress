//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/ServerMessage.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerMessageProcessor
{
public:
  ServerMessageProcessor(ServerMessage serverMessage) :
    m_serverMessage(serverMessage)
  {
  }

protected:
  ~ServerMessageProcessor() {};

  boost::asio::io_service & GetIoService() const;

  AsioExpress::MessagePort::DataBufferPointer GetDataBuffer() const;

  MessagePortId GetMessagePortId() const;

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

protected:
  ServerMessage       m_serverMessage;
};

inline boost::asio::io_service & 
ServerMessageProcessor::GetIoService() const
{
  return m_serverMessage.GetIoService();
}

inline AsioExpress::MessagePort::DataBufferPointer 
ServerMessageProcessor::GetDataBuffer() const
{
  return m_serverMessage.GetDataBuffer();
}

inline MessagePortId ServerMessageProcessor::GetMessagePortId() const
{
  return m_serverMessage.GetMessagePortId();
}

template<typename H>
void ServerMessageProcessor::AsyncSend(
    MessagePortId id, 
    DataBufferPointer buffer, 
    H completionHandler)
{
  m_serverMessage.AsyncSend(id, buffer, completionHandler);
}

template<typename H>
void ServerMessageProcessor::AsyncBroadcast(
    DataBufferPointer buffer, 
    H completionHandler)
{
  m_serverMessage.AsyncBroadcast(buffer, completionHandler);
}

inline void ServerMessageProcessor::CallCompletionHandler(
    AsioExpress::Error const & error)
{
  m_serverMessage.CallCompletionHandler(error);
}

} // namespace MessagePort
} // namespace AsioExpress
