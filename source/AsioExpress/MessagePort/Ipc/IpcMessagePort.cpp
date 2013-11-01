//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpress/Platform/DebugMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/IpcErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/IpcMessagePort.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandConnect.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandReceive.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {


IpcMessagePort::IpcMessagePort(boost::asio::io_service & ioService) :
  m_ioService(ioService)
{
}


IpcMessagePort::~IpcMessagePort()
{
  Disconnect();
}


void IpcMessagePort::Disconnect()
{
  // Before allowing a disconnect, make sure any threads are completed.
  //
  if (m_receiveThread)
    m_receiveThread->Close();

  if (m_sendThread)
    m_sendThread->Close();

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
    m_sendMessageQueueName = "";
  }

  if ( !m_recvMessageQueueName.empty() )
  {
    boost::interprocess::message_queue::remove(m_recvMessageQueueName.c_str());
    m_recvMessageQueueName = "";
  }
}


void IpcMessagePort::AsyncConnect(
    IpcEndPoint endPoint, 
    AsioExpress::CompletionHandler completionHandler)
{
  IpcCommandConnect(endPoint, 
                    *this, 
                    completionHandler)();
}


void IpcMessagePort::AsyncSend(
    AsioExpress::MessagePort::DataBufferPointer buffer,
    AsioExpress::CompletionHandler completionHandler)
{
  // Check that we're connected
#ifdef DEBUG_IPC
  DebugMessage("IpcMessagePort::AsyncSend: Sending message.\n");
#endif

  if ( !m_sendMessageQueue )
  {
#ifdef DEBUG_IPC
    DebugMessage("IpcMessagePort::AsyncSend: No connection has been established!\n");
#endif

    AsioExpress::Error err(
      ErrorCode::Disconnected,
      "IpcMessagePort::AsyncSend(): No connection has been established.");
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, err));
    return;
  }

  // Send the message or fail if queue is full
  try
  {
    m_sendThread->AsyncSend(
      buffer, 
      0, 
      completionHandler);
  }
  catch(AsioExpress::CommonException const & e)
  {
    Disconnect();
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, e.GetError()));
  }
}

void IpcMessagePort::AsyncReceive(
    AsioExpress::MessagePort::DataBufferPointer buffer,
    AsioExpress::CompletionHandler completionHandler)
{
  // Check that we're connected

  if ( !m_recvMessageQueue )
  {
    AsioExpress::Error err(
      ErrorCode::Disconnected,
      "IpcMessagePort::AsyncReceive(): No connection has been established.");
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, err));
    return;
  }

  // Receive the next message & copy to the buffer

  IpcCommandReceive(m_ioService,
                    m_receiveThread,
                    m_recvMessageQueue,
                    buffer,
                    completionHandler,
                    0)();
}


AsioExpress::Error IpcMessagePort::SetupWithMessageQueues(const std::string& sendQueue, const std::string& recvQueue)
{
  Disconnect();

  try 
  {
    m_sendMessageQueueName = sendQueue;
    m_recvMessageQueueName = recvQueue;    
    m_sendMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::open_only, m_sendMessageQueueName.c_str()));
    m_recvMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::open_only, m_recvMessageQueueName.c_str()));
    m_receiveThread.reset(new IpcReceiveThread(m_ioService, m_recvMessageQueue));
    m_sendThread.reset(new IpcSendThread(m_ioService, m_sendMessageQueue));
  }
  catch(boost::interprocess::interprocess_exception& ex) 
  {
    Disconnect();
    AsioExpress::Error err(boost::system::error_code(
      ex.get_native_error(), boost::system::get_system_category()),
      "IpcMessagePort::SetupWithMessageQueues(): Unable to open client/server message queues.");    
    return err;
  }

  return AsioExpress::Error();
}

void IpcMessagePort::SetMessagePortOptions()
{
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
