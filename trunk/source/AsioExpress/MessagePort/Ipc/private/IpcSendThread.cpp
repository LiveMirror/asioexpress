//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <boost/chrono.hpp>

#include "AsioExpressConfig/config.hpp"
#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSendThread.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcConstants.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

WIN_DISABLE_WARNINGS_BEGIN(4355)
IpcSendThread::IpcSendThread(
    boost::asio::io_service & ioService,
    MessageQueuePointer messageQueue,
    PingMode pingMode) :
  m_ioService(ioService),
  m_messageQueue(messageQueue),
  m_isClosing(false),
  m_sendFailed(false),
  m_alertThrown(false),
  m_pingMode(pingMode),
  m_thread(boost::bind(&IpcSendThread::SendFunction, this))
{
}
WIN_DISABLE_WARNINGS_END

IpcSendThread::~IpcSendThread()
{
  Close();
}

void IpcSendThread::AsyncSend(
    DataBufferPointer dataBuffer, 
    unsigned int priority,
    AsioExpress::CompletionHandler completionHandler)
{
  if (m_sendFailed)
  {
    throw AsioExpress::CommonException(
      AsioExpress::Error(
        ErrorCode::MessageQueueSendFailed,
        "IpcSendThread(): Message queue send call failed."));
  }

  if (m_isClosing)
  {
    AsioExpress::Error err(
      boost::asio::error::operation_aborted,
      "IpcSendThread(): Send was canceled.");
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

void IpcSendThread::Close()
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

void IpcSendThread::SendFunction()
{
  boost::unique_lock<boost::mutex> alertLock(m_alertMutex);
  
  ResetPingTime();
 
  while(! m_isClosing)
  {
    if (m_alertThrown)
    {
      ResetPingTime();
      
      Send();          

      m_alertThrown = false;
    }
    else if (IsPingTime())
    {      
#ifdef DEBUG_IPC
      DebugMessage("IpcSendThread: Sending ping on idle connection.\n");
#endif
      ResetPingTime();      

      SendSystemMessage(IpcSysMessage::MSG_PING);      
    }

    boost::chrono::system_clock::time_point now(
        boost::chrono::system_clock::now());

    m_alert.wait_until(
            alertLock, 
            now + (m_pingTime - now));
  }

  Send();

  if (m_sendFailed)
    return;

  // Send disconnect message.
  //
  SendSystemMessage(IpcSysMessage::MSG_DISCONNECT);
}

void IpcSendThread::Send()
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
        "IpcSendThread(): Message queue send call failed."));
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(CallCompletionHandlers(p, end, error))
}

void IpcSendThread::Send(SendParameters const & parameters)
{
  bool successful = m_messageQueue->try_send(
    parameters.dataBuffer->Get(), 
    parameters.dataBuffer->Size(), 
    parameters.priority);

  if (!successful)
  {
#ifdef DEBUG_IPC
    DebugMessage("MessagePort::AsyncSend: Send error!\n");
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

void IpcSendThread::CallCompletionHandlers(
    SendQueue::iterator parameters,
    SendQueue::iterator end,
    AsioExpress::Error error)
{
  for (; parameters != end; ++parameters)
  {
    CallCompletionHandler(parameters->completionHandler, error);
  }
}

void IpcSendThread::CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    boost::system::error_code errorCode,
    std::string message)
{
  CallCompletionHandler(completionHandler, AsioExpress::Error(errorCode, message));
}

void IpcSendThread::CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    AsioExpress::Error error)
{
  m_ioService.post(boost::asio::detail::bind_handler(completionHandler, error));
}

void IpcSendThread::SendSystemMessage(char const * systemMessage)
{
  IpcSysMessage msg(systemMessage);
  DataBuffer dataBuffer(msg.RequiredEncodeBufferSize());
  (void)msg.Encode(dataBuffer.Get());
  try
  {
    (void)m_messageQueue->try_send(
      dataBuffer.Get(), 
      dataBuffer.Size(), 
      IpcSysMessage::SYS_MSG_PRIORITY);
  }
  catch(boost::interprocess::interprocess_exception &) 
  {    
    // ignore any error
  }    
}

void IpcSendThread::ResetPingTime()
{
  if (m_pingMode == DisablePing)
    return;
  
  m_pingTime = boost::chrono::system_clock::now() + boost::chrono::seconds(PingRateSeconds);
}

bool IpcSendThread::IsPingTime()
{
  if (m_pingMode == DisablePing)
    return false; 
  
  return boost::chrono::system_clock::now() >= m_pingTime;
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
