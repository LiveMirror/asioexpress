//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"

#include "AsioExpress/MessagePort/SyncIpc/private/MessageQueuePointer.hpp"
#include "AsioExpress/MessagePort/SyncIpc/SyncIpcEndPoint.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

class SyncIpcMessagePort
{
  friend void SyncIpcCommandConnect(SyncIpcEndPoint const & endPoint, SyncIpcMessagePort & messagePort);

public:
  typedef SyncIpcEndPoint EndPointType;

public:
  ~SyncIpcMessagePort();

  void Connect(
      SyncIpcEndPoint endPoint);

  void Send(
      AsioExpress::MessagePort::DataBufferPointer buffer);

  void Receive(
      AsioExpress::MessagePort::DataBufferPointer buffer);
  
  bool Receive(
      AsioExpress::MessagePort::DataBufferPointer buffer,
      int maxMilliseconds);
  
  void Disconnect();

  void SetMessagePortOptions();

  inline bool IsConnected() const               { return m_sendMessageQueue != 0; }
  inline const std::string& GetLocalID() const  { return m_recvMessageQueueName; }
  inline const std::string& GetRemoteID() const { return m_sendMessageQueueName; }

private:
  SyncIpcMessagePort & operator=(SyncIpcMessagePort const &);
  AsioExpress::Error SetupWithMessageQueues(const std::string& sendQueue, const std::string& recvQueue);

private:
  std::string                             m_sendMessageQueueName;
  std::string                             m_recvMessageQueueName;
  MessageQueuePointer                     m_sendMessageQueue;
  MessageQueuePointer                     m_recvMessageQueue;
};

} // namespace SyncIpc
} // namespace MessagePort
} // namespace AsioExpress
