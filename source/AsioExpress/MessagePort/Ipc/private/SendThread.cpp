//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/pch.hpp"

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/MessagePort/Ipc/private/SendThread.hpp"
#include "AsioExpress/MessagePort/Ipc/IpcErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/MessagePortSysMessage.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

#pragma warning(push)
#pragma warning(disable: 4355)
SendThread::SendThread(
    boost::asio::io_service & ioService,
    MessageQueuePointer messageQueue) :
  m_ioService(ioService),
  m_messageQueue(messageQueue),
  m_isClosing(false),
  m_sendFailed(false),
  m_alertThrown(false),
  m_thread(boost::bind(&SendThread::SendFunction, this))
{
}
#pragma warning(pop)

SendThread::~SendThread()
{
  Close();
}

void SendThread::AsyncSend(
    DataBufferPointer dataBuffer, 
    unsigned int priority,
    AsioExpress::CompletionHandler completionHandler)
{
  if (m_sendFailed)
  {
    throw AsioExpress::CommonException(
      AsioExpress::Error(
        ErrorCode::MessageQueueSendFailed,
        "SendThread(): Message queue send call failed."));
  }

  if (m_isClosing)
  {
    AsioExpress::Error err(
      boost::asio::error::operation_aborted,
      "IPC SendThread(): Send was canceled.");
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, err));
    return;
  }

  SendParameters parameters;
  parameters.dataBuffer = dataBuffer;
  parameters.priority = priority;
  parameters.completionHandler = completionHandler;

  // insert into queue
  {
    boost::mutex::scoped_lock lock(m_sendQueueMutex);
    m_sendQueue.push_back(parameters);
  }

  // notify thread of state change
  {
    boost::mutex::scoped_lock lock(m_alertMutex);
    m_alertThrown = true;
    m_alert.notify_one();
  }
}

void SendThread::Close()
{
  m_isClosing = true;

  // notify thread of state change
  {
    boost::mutex::scoped_lock lock(m_alertMutex);
    m_alertThrown = true;
    m_alert.notify_one();
  }

  m_thread.join();
}

void SendThread::SendFunction()
{
  boost::unique_lock<boost::mutex> alertLock(m_alertMutex);

  while(! m_isClosing)
  {
    if (m_alertThrown)
    {
      Send();          

      m_alertThrown = false;
    }

    m_alert.wait(alertLock);
  }

  Send();

  if (m_sendFailed)
    return;

  // Send disconnect message.
  //
  MessagePortSysMessage msg(MessagePortSysMessage::MSG_DISCONNECT);
  DataBuffer dataBuffer(msg.RequiredEncodeBufferSize());
  (void)msg.Encode(dataBuffer.Get());
  try
  {
    (void)m_messageQueue->try_send(
      dataBuffer.Get(), 
      dataBuffer.Size(), 
      MessagePortSysMessage::SYS_MSG_PRIORITY);
  }
  catch(boost::interprocess::interprocess_exception &) 
  {    
    // ignore any error
  }
}

void SendThread::Send()
{
  SendQueue sendQueueCopy;

  // copy send queue
  {
    boost::mutex::scoped_lock lock(m_sendQueueMutex);
    sendQueueCopy = m_sendQueue;
    m_sendQueue.clear();
  }

  SendQueue::iterator   p = sendQueueCopy.begin();
  SendQueue::iterator end = sendQueueCopy.end();

  try
  {
    for (; p != end; ++p)
    {
      Send(*p);
    }
  }
  catch(boost::interprocess::interprocess_exception &) 
  {    
    m_sendFailed =  true;

    // Some kind of error      
    CallCompletionHandlers(
      p, end,
      AsioExpress::Error(
        ErrorCode::MessageQueueSendFailed,
        "SendThread(): Message queue send call failed."));
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(CallCompletionHandlers(p, end, error))
}

void SendThread::Send(SendParameters const & parameters)
{
  bool successful = m_messageQueue->try_send(
    parameters.dataBuffer->Get(), 
    parameters.dataBuffer->Size(), 
    parameters.priority);

  if (!successful)
  {
#ifdef DEBUG_IPC
    OutputDebugString("IPC::MessagePort::AsyncSend: Send error!\n");
#endif
    CallCompletionHandler(
      parameters.completionHandler,
      ErrorCode::MessageQueueFull,
      "MessagePort::AsyncSend(): Recipient's message queue is full.");
    return;
  }

  CallCompletionHandler(
    parameters.completionHandler,
    AsioExpress::Error());
}

void SendThread::CallCompletionHandlers(
    SendQueue::iterator parameters,
    SendQueue::iterator end,
    AsioExpress::Error error)
{
  for (; parameters != end; ++parameters)
  {
    CallCompletionHandler(parameters->completionHandler, error);
  }
}

void SendThread::CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    boost::system::error_code errorCode,
    std::string message)
{
  CallCompletionHandler(completionHandler, AsioExpress::Error(errorCode, message));
}

void SendThread::CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    AsioExpress::Error error)
{
  m_ioService.post(boost::asio::detail::bind_handler(completionHandler, error));
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
