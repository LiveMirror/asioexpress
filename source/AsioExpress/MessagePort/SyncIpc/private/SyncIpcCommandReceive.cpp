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
#include "AsioExpress/MessagePort/SyncIpc/private/SyncIpcCommandReceive.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

static bool Receive(
    MessageQueuePointer messageQueue,
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

      if (successful)
      {
        // Successful receive, copy to buffer and post callback with no error

        dataBuffer->Resize(recvSize);
        memcpy(dataBuffer->Get(), tempBuffer.Get(), recvSize);
        
        break;
      }
      else if ( hasTimeout && expiryTime <= nextTimeout )
      {
        return false;
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
    DebugMessage("MessagePortCommandReceive: Waiting to receive message.\n");
#endif

    unsigned int priority = 0;
    if (! Receive(messageQueue, dataBuffer, priority, maxMilliseconds))
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
            DebugMessage("MessagePortCommandReceive: A disconnect message was received.\n");
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
    DebugMessage("MessagePortCommandReceive: Message received.\n");
#endif

    return true;
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
