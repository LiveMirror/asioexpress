//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ClientServer/ClientMessage.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientMessageProcessor
{
public:
  ClientMessageProcessor(ClientMessage clientMessage) :
    m_clientMessage(clientMessage)
  {
  }

protected:
  ~ClientMessageProcessor() {};

  boost::asio::io_service & GetIoService() const;

  AsioExpress::MessagePort::DataBufferPointer GetDataBuffer() const;

  MessagePortId GetMessagePortId() const;

  ClientInterfacePointer GetMessagePortClient() const;

  template<typename H>
  void AsyncSend(
      DataBufferPointer buffer, 
      H completionHandler);

  void CallCompletionHandler(AsioExpress::Error const & error);

 protected:
  ClientMessage                     m_clientMessage;
};

inline boost::asio::io_service & 
ClientMessageProcessor::GetIoService() const
{
  return m_clientMessage.GetIoService();
}

inline AsioExpress::MessagePort::DataBufferPointer 
ClientMessageProcessor::GetDataBuffer() const
{
  return m_clientMessage.GetDataBuffer();
}

inline MessagePortId ClientMessageProcessor::GetMessagePortId() const
{
  return m_clientMessage.GetMessagePortId();
}
  
inline ClientInterfacePointer ClientMessageProcessor::GetMessagePortClient() const
{
  return m_clientMessage.GetMessagePortClient();
}

template<typename H>
void ClientMessageProcessor::AsyncSend(
    DataBufferPointer buffer, 
    H completionHandler)
{
  m_clientMessage.AsyncSend(buffer, completionHandler);
}

inline void ClientMessageProcessor::CallCompletionHandler(
    AsioExpress::Error const & error)
{
  m_clientMessage.CallCompletionHandler(error);
}

} // namespace MessagePort
} // namespace AsioExpress
