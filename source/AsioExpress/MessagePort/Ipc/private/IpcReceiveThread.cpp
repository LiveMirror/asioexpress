//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcReceiveThread.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcConstants.hpp"
#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

WIN_DISABLE_WARNINGS_BEGIN(4355)
IpcReceiveThread::IpcReceiveThread(
    boost::asio::io_service & ioService,
    MessageQueuePointer messageQueue,
    PingMode pingMode) :
  m_ioService(ioService),
  m_messageQueue(messageQueue),
  m_isReceiving(false),
  m_isCanceled(false),
  m_isClosing(false),
  m_alertThrown(false),
  m_pingMode(pingMode),
  m_thread(boost::bind(&IpcReceiveThread::ReceiveFunction, this))
{
}
WIN_DISABLE_WARNINGS_END

IpcReceiveThread::~IpcReceiveThread()
{
  Close();
}

void IpcReceiveThread::AsyncReceive(
    DataBufferPointer dataBuffer, 
    boost::shared_ptr<unsigned int> priority,
    AsioExpress::CompletionHandler completionHandler, 
    int maxMilliseconds)
{
  if (m_isReceiving)
  {
    AsioExpress::Error err(
      ErrorCode::BadUsage,
      "MessagePortCommandReceive(): A previous receive has not yet completed.");
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, err));
    return;
  }

  if (m_isClosing)
  {
    AsioExpress::Error err(
      boost::asio::error::operation_aborted,
      "IpcReceiveThread: Receive was canceled.");
    m_ioService.post(boost::asio::detail::bind_handler(completionHandler, err));
    return;
  }

  m_isCanceled = false;

  m_parameters.dataBuffer = dataBuffer;
  m_parameters.priority = priority;
  m_parameters.completionHandler = completionHandler;
  m_parameters.maxMilliseconds = maxMilliseconds;

  // notify thread of state change
  {
    boost::mutex::scoped_lock lock(m_alertMutex);
    m_alertThrown = true;
    m_alert.notify_one();
  }
}

void IpcReceiveThread::CancelReceive()
{
  m_isCanceled = true;
}

void IpcReceiveThread::Close()
{
  m_isCanceled = true;
  m_isClosing = true;

  // notify thread of state change
  {
    boost::mutex::scoped_lock lock(m_alertMutex);
    m_alertThrown = true;
    m_alert.notify_one();
  }

  m_thread.join();
}

void IpcReceiveThread::ReceiveFunction()
{
  boost::unique_lock<boost::mutex> alertLock(m_alertMutex);

  while(! m_isClosing)
  {
    if (m_alertThrown)
    {
      try
      {
        Receive();          
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO(CallCompletionHandler(error))

      m_alertThrown = false;
    }

    m_alert.wait(alertLock);
  }
}

void IpcReceiveThread::Receive()
{
  m_isReceiving = true;

  bool hasTimeout = (m_parameters.maxMilliseconds > 0);
  boost::posix_time::ptime expiryTime;
  
  if ( hasTimeout )
  {
    expiryTime 
      = boost::posix_time::microsec_clock::universal_time() 
        + boost::posix_time::milliseconds(m_parameters.maxMilliseconds);
  }
  
  ResetPingTimeout();
  
  // Now continuously call the timed receive

  unsigned int priority;
  std::size_t recvSize;
  DataBuffer tempBuffer(m_messageQueue->get_max_msg_size());

  boost::posix_time::ptime nextTimeout;

  for (;;)
  {
    if (m_isCanceled)
    {
      CallCompletionHandler(
        boost::asio::error::operation_aborted,
        "IpcReceiveThread: Receive was canceled.");
      break;
    }

    nextTimeout 
      = boost::posix_time::microsec_clock::universal_time() 
        + boost::posix_time::milliseconds(1000);

    if ( hasTimeout && nextTimeout > expiryTime )
      nextTimeout = expiryTime;

    try 
    {
      bool successful = m_messageQueue->timed_receive(
        tempBuffer.Get(), 
        tempBuffer.Size(), 
        recvSize, 
        priority, 
        nextTimeout);

      if ( successful && priority == IpcSysMessage::SYS_MSG_PRIORITY )
      {
        IpcSysMessage msg;
        msg.Decode(tempBuffer.Get());

        if ( msg.GetMessageType() == IpcSysMessage::MSG_PING )
        {
  #ifdef DEBUG_IPC
          DebugMessage("IpcReceiveThread: Ping message received.\n");
  #endif
          ResetPingTimeout();          
          continue;
        }
      }
      
      if ( successful )
      {
        ResetPingTimeout();
        
        // Successful receive, copy to buffer and post callback with no error

        *(m_parameters.priority) = priority;
        m_parameters.dataBuffer->Resize(recvSize);
        memcpy(m_parameters.dataBuffer->Get(), tempBuffer.Get(), recvSize);

        CallCompletionHandler(AsioExpress::Error());
        break;
      }
      
      if ( !successful && hasTimeout && expiryTime <= nextTimeout )
      {
        // Our overall timeout period elapsed

        CallCompletionHandler(
          ErrorCode::TimeOutExpired,
          "IpcReceiveThread: Connect/receive request timed out.");
        break;
      }
      
      if ( !successful && PingTimeout() )
      {
        // Our overall timeout period elapsed

        CallCompletionHandler(
          ErrorCode::LostConnection,
          "IpcReceiveThread: No ping message received.");
        break;
      }
      
    }
    catch(boost::interprocess::interprocess_exception &ex) 
    {    
      // Some kind of error      
      CallCompletionHandler(
        boost::system::error_code(ex.get_native_error(), boost::system::get_system_category()),
        "IpcReceiveThread: Message queue receive call failed.");
      break;
    }
  }    
}

void IpcReceiveThread::CallCompletionHandler(
    boost::system::error_code errorCode,
    std::string message)
{
  CallCompletionHandler(AsioExpress::Error(errorCode, message));
}

void IpcReceiveThread::CallCompletionHandler(
    AsioExpress::Error err)
{
  m_isReceiving = false;

  AsioExpress::CompletionHandler handler(m_parameters.completionHandler); 

  // we need to reset the completion handler so it does not linger.
  m_parameters.completionHandler = 0;

  m_ioService.post(boost::asio::detail::bind_handler(handler, err));
}

void IpcReceiveThread::ResetPingTimeout()
{
  if (m_pingMode == DisablePing)
    return;
    
  m_pingTimeout 
      = boost::posix_time::microsec_clock::universal_time() 
        + boost::posix_time::seconds(PingTimeoutSeconds);
}

bool IpcReceiveThread::PingTimeout()
{
  if (m_pingMode == DisablePing)
    return false; 
  
  return boost::posix_time::microsec_clock::universal_time() > m_pingTimeout;
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
