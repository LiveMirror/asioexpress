//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpressError/CommonException.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/IpcErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/MessagePort/SyncIpc/SyncIpcMessagePort.hpp"
#include "AsioExpress/MessagePort/SyncIpc/private/SyncIpcCommandReceive.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

SyncIpcMessagePort::~SyncIpcMessagePort()
{
  Disconnect();
}

void SyncIpcMessagePort::Disconnect()
{
  using namespace AsioExpress::MessagePort::Ipc;
  
  // Send disconnect message.
  //
  if (m_sendMessageQueue)
  {
    IpcSysMessage msg(IpcSysMessage::MSG_DISCONNECT);
    DataBuffer dataBuffer(msg.RequiredEncodeBufferSize());
    (void)msg.Encode(dataBuffer.Get());
    try
    {
      (void)m_sendMessageQueue->try_send(
        dataBuffer.Get(), 
        dataBuffer.Size(), 
        IpcSysMessage::SYS_MSG_PRIORITY);
    }
    catch(boost::interprocess::interprocess_exception &) 
    {    
      // ignore any error
    } 
  }
  
  // Queues can be just deleted and removed
  //
  if ( m_recvMessageQueue )
  {
    m_recvMessageQueue.reset();
  }

  if ( m_sendMessageQueue )
  {
    m_recvMessageQueue.reset();
  }

  // Delete the queues from the system
  //
  if ( !m_sendMessageQueueName.empty() )
  {
    boost::interprocess::message_queue::remove(m_sendMessageQueueName.c_str());
    m_sendMessageQueueName.clear();
  }

  if ( !m_recvMessageQueueName.empty() )
  {
    boost::interprocess::message_queue::remove(m_recvMessageQueueName.c_str());
    m_recvMessageQueueName.clear();
  }
}

void SyncIpcMessagePort::Connect(
    SyncIpcEndPoint endPoint)
{
  SyncIpcCommandConnect(endPoint, *this);
}

void SyncIpcMessagePort::Send(
    AsioExpress::MessagePort::DataBufferPointer buffer)
{
  using namespace AsioExpress;
    
  // Check that we're connected
#ifdef DEBUG_IPC
  DebugMessage("IPC::SyncIpcMessagePort::AsyncSend: Sending message.\n");
#endif

  if ( !m_sendMessageQueue )
  {
#ifdef DEBUG_IPC
    DebugMessage("IPC::SyncIpcMessagePort::AsyncSend: No connection has been established!\n");
#endif

    throw CommonException(Error(
          AsioExpress::MessagePort::Ipc::ErrorCode::Disconnected,
          "SyncIpcMessagePort::Send(): No connection has been established."));
  }

  // Send the message or fail if queue is full
  try
  {
    bool successful = m_sendMessageQueue->try_send(
      buffer->Get(), 
      buffer->Size(),
      0);

    if (!successful)
    {
  #ifdef DEBUG_IPC
      DebugMessage("IPC::SyncIpcMessagePort::AsyncSend: Send error!\n");
  #endif

      throw CommonException(Error(
        AsioExpress::MessagePort::Ipc::ErrorCode::MessageQueueFull,
        "SyncIpcMessagePort::Send(): Recipient's message queue is full."));
      return;
    } 
    
  }
  catch(AsioExpress::CommonException const & e)
  {
    Disconnect();
    throw;
  }
}
    
void SyncIpcMessagePort::Receive(
    AsioExpress::MessagePort::DataBufferPointer buffer)
{
    (void) Receive(buffer, 0);
}

bool SyncIpcMessagePort::Receive(
    AsioExpress::MessagePort::DataBufferPointer buffer,
    int maxMilliseconds)
{
  using namespace AsioExpress::MessagePort::Ipc;

  // Check that we're connected

  if ( !m_recvMessageQueue )
  {
    throw CommonException(Error(
      AsioExpress::MessagePort::Ipc::ErrorCode::Disconnected,
      "SyncIpcMessagePort::Receive(): No connection has been established."));
  }

  // Receive the next message & copy to the buffer

  return SyncIpcCommandReceive(
                m_recvMessageQueue,
                buffer,
                maxMilliseconds);
}


AsioExpress::Error SyncIpcMessagePort::SetupWithMessageQueues(const std::string& sendQueue, const std::string& recvQueue)
{
  Disconnect();

  try 
  {
    m_sendMessageQueueName = sendQueue;
    m_recvMessageQueueName = recvQueue;    
    m_sendMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::open_only, m_sendMessageQueueName.c_str()));
    m_recvMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::open_only, m_recvMessageQueueName.c_str()));
  }
  catch(boost::interprocess::interprocess_exception& ex) 
  {
    Disconnect();
    AsioExpress::Error err(
        boost::system::error_code(
            ex.get_native_error(),
            boost::system::get_system_category()),
        "SyncIpcMessagePort::SetupWithMessageQueues(): Unable to open client/server message queues.");    
    return err;
  }

  return AsioExpress::Error();
}

void SyncIpcMessagePort::SetMessagePortOptions()
{
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
