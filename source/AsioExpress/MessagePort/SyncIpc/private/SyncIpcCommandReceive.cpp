//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpressError/CommonException.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcConstants.hpp"
#include "AsioExpress/MessagePort/SyncIpc/private/SyncIpcCommandReceive.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

class PingTimer
{
public:
    void Reset()
    {
      pingTimeout 
          = boost::posix_time::microsec_clock::universal_time() 
            + boost::posix_time::seconds(Ipc::PingTimeoutSeconds);
    }

    bool Elapsed()
    {
      return boost::posix_time::microsec_clock::universal_time() > pingTimeout;
    }    
private:
    boost::posix_time::ptime    pingTimeout;
};

static void SendSystemMessage(
    MessageQueuePointer sendMessageQueue,
    char const * systemMessage)
{
  Ipc::IpcSysMessage msg(systemMessage);
  DataBuffer dataBuffer(msg.RequiredEncodeBufferSize());
  (void)msg.Encode(dataBuffer.Get());
  try
  {
    (void)sendMessageQueue->try_send(
      dataBuffer.Get(), 
      dataBuffer.Size(), 
      Ipc::IpcSysMessage::SYS_MSG_PRIORITY);
  }
  catch(boost::interprocess::interprocess_exception &) 
  {    
    // ignore any error
  }    
}

static bool Receive(
    MessageQueuePointer messageQueue,
    MessageQueuePointer sendMessageQueue,
    DataBufferPointer dataBuffer,
    unsigned int & priority,
    int maxMilliseconds)
{
  using namespace AsioExpress;
  using namespace AsioExpress::MessagePort::Ipc;
  
  bool hasTimeout = (maxMilliseconds > 0);
  boost::posix_time::ptime expiryTime;
  
  if ( hasTimeout )
  {
    expiryTime 
      = boost::posix_time::microsec_clock::universal_time() 
        + boost::posix_time::milliseconds(maxMilliseconds);
  }
    
  // Now continuously call the timed receive

  std::size_t recvSize = 0;
  DataBuffer tempBuffer(messageQueue->get_max_msg_size());

  boost::posix_time::ptime nextTimeout;
  
  PingTimer pingTimer;

  for (;;)
  {
    nextTimeout 
      = boost::posix_time::microsec_clock::universal_time() 
        + boost::posix_time::milliseconds(1000);

    if ( hasTimeout && nextTimeout > expiryTime )
      nextTimeout = expiryTime;

    try 
    {
      bool successful = messageQueue->timed_receive(
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
          DebugMessage("SyncIpcCommandReceive: Ping message received.\n");
  #endif
          pingTimer.Reset();          
          
          SendSystemMessage(sendMessageQueue, IpcSysMessage::MSG_PING);                
          continue;
        }
      }
      
      if ( successful )
      {
        // Successful receive, copy to buffer and post callback with no error
          
        pingTimer.Reset();
        
        dataBuffer->Resize(recvSize);
        memcpy(dataBuffer->Get(), tempBuffer.Get(), recvSize);        
        
        break;
      }
      
      if ( !successful && hasTimeout && expiryTime <= nextTimeout )
      {
        // Our overall timeout period elapsed
        return false;
      }
      
      if ( !successful && pingTimer.Elapsed() )
      {
        // Our overall timeout period elapsed

        throw CommonException(Error(
          ErrorCode::LostConnection,
          "IpcReceiveThread: No ping message received."));
        break;
      }      
    }
    catch(boost::interprocess::interprocess_exception &ex) 
    {    
      // Some kind of error
      throw CommonException(Error(
        boost::system::error_code(ex.get_native_error(), boost::system::get_system_category()),
        "MossagePort::Receive(): Message queue receive call failed."));
    }
  }
  
  return true;
}

bool SyncIpcCommandReceive(
    MessageQueuePointer messageQueue,
    MessageQueuePointer sendMessageQueue,
    DataBufferPointer dataBuffer,
    int maxMilliseconds)
{
    using namespace AsioExpress;
    using namespace AsioExpress::MessagePort::Ipc;
    
    // 
    // Step 1 - Create a temporary MessagePortCommandReceiveThread instance to
    //          do the actual receive in a new thread.
    //

#ifdef DEBUG_IPC
    DebugMessage("SyncIpcCommandReceive: Waiting to receive message.\n");
#endif

    unsigned int priority = 0;
    if (! Receive(messageQueue, sendMessageQueue, dataBuffer, priority, maxMilliseconds))
        return false;

    //
    // Step 2 - If this is a standard message port, check for a disconnect message
    //

    if (priority == IpcSysMessage::SYS_MSG_PRIORITY)
    {
        IpcSysMessage msg;
        msg.Decode(dataBuffer->Get());

        if (msg.GetMessageType() == IpcSysMessage::MSG_DISCONNECT)
        {
            // This is a disconnect message; return an error.

#ifdef DEBUG_IPC
            DebugMessage("SyncIpcCommandReceive: A disconnect message was received.\n");
#endif

            throw CommonException(Error(
                    ErrorCode::Disconnected,
                    "MessagePort::AsyncReceive(): Connection was disconnected by peer."));
        }
    }

    // 
    // Step 3 - Receive was successful, invoke the callback.
    //
#ifdef DEBUG_IPC
    DebugMessage("SyncIpcCommandReceive: Message received.\n");
#endif

    return true;
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
