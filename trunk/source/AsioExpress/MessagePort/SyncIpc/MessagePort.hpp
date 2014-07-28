//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"

#include "AsioExpress/MessagePort/SyncIpc/private/MessageQueuePointer.hpp"
#include "AsioExpress/MessagePort/SyncIpc/EndPoint.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

class MessagePort
{
  friend void SyncIpcCommandConnect(EndPoint const & endPoint, MessagePort & messagePort);

public:
  typedef EndPoint EndPointType;

public:
  ~MessagePort();

  void Connect(
      EndPoint endPoint);

  void Send(
      AsioExpress::MessagePort::DataBufferPointer buffer);

  /* NOTE: use only for testing */
  void TestSend(AsioExpress::MessagePort::DataBufferPointer buffer,
      MessageQueuePointer sendQueuePointer);

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
  MessagePort & operator=(MessagePort const &);
  AsioExpress::Error SetupWithMessageQueues(const std::string& sendQueue, const std::string& recvQueue);

private:
  void InternalDisconnect();
  
  std::string                             m_sendMessageQueueName;
  std::string                             m_recvMessageQueueName;
  boost::mutex                            m_sendMutex;
  MessageQueuePointer                     m_sendMessageQueue;
  boost::mutex                            m_recvMutex;
  MessageQueuePointer                     m_recvMessageQueue;
};

} // namespace SyncIpc
} // namespace MessagePort
} // namespace AsioExpress
