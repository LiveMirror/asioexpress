//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpressError/CommonException.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"
#include "AsioExpress/Platform/Sleep.hpp"
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
    PingTimer()
    {
        Reset();
    }

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

class ReceiveTimeout
{
public:
    ReceiveTimeout(int maxMilliseconds) :
        hasTimeout(maxMilliseconds > 0)
    {
      if (hasTimeout)
      {
        expiryTime
          = boost::posix_time::microsec_clock::universal_time()
            + boost::posix_time::milliseconds(maxMilliseconds);
      }
    }

    boost::posix_time::ptime GetNextTimeout()
    {
      nextTimeout
        = boost::posix_time::microsec_clock::universal_time()
          + boost::posix_time::milliseconds(1000);

      if ( hasTimeout && nextTimeout > expiryTime )
        nextTimeout = expiryTime;

      return nextTimeout;
    }

    bool Elapsed() const
    {
      return hasTimeout && expiryTime <= nextTimeout;
    }

private:
    bool hasTimeout;
    boost::posix_time::ptime expiryTime;
    boost::posix_time::ptime nextTimeout;
};

static void SendSystemMessage(
    MessageQueuePointer sendMessageQueue,
    boost::mutex & sendMutex,
    char const * systemMessage)
{
  Ipc::IpcSysMessage msg(systemMessage);
  DataBuffer dataBuffer(msg.RequiredEncodeBufferSize());
  (void)msg.Encode(dataBuffer.Get());
  try
  {
    boost::mutex::scoped_lock sendLock(sendMutex);

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
    MessageQueuePointer recvMessageQueue,
    MessageQueuePointer sendMessageQueue,
    boost::mutex & recvMutex,
    boost::mutex & sendMutex,
    DataBufferPointer dataBuffer,
    unsigned int priority,
    int maxMilliseconds)
{
  using namespace AsioExpress;
  using namespace AsioExpress::MessagePort::Ipc;

  ReceiveTimeout timeout(maxMilliseconds);

  std::size_t recvSize = 0;
  size_t maxMessageSize = recvMessageQueue->get_max_msg_size();

  DataBuffer tempBuffer(maxMessageSize);

  PingTimer pingTimer;

  // Now continuously call the timed receive

  for (;;)
  {
    try
    {
      bool successful(false);
      {
        boost::mutex::scoped_lock recvLock(recvMutex);

        successful = recvMessageQueue->timed_receive(
                tempBuffer.Get(),
                tempBuffer.Size(),
                recvSize,
                priority,
                timeout.GetNextTimeout());
      }

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

          SendSystemMessage(sendMessageQueue, sendMutex, IpcSysMessage::MSG_PING);
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

      if ( !successful && timeout.Elapsed() )
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

      // We need a sleep to force context switch.
      AsioExpress::Sleep(20);
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
    MessageQueuePointer recvMessageQueue,
    DataBufferPointer dataBuffer,
    int maxMilliseconds)
{
  using namespace AsioExpress;
  using namespace AsioExpress::MessagePort::Ipc;

  unsigned int priority = 0;

  ReceiveTimeout timeout(maxMilliseconds);

  std::size_t recvSize = 0;
  size_t maxMessageSize = recvMessageQueue->get_max_msg_size();

  DataBuffer tempBuffer(maxMessageSize);

  boost::posix_time::ptime nextTimeout;

  // Now continuously call the timed receive
  for (;;)
  {
    try
    {
      bool successful(false);
      {
        successful = recvMessageQueue->timed_receive(
                tempBuffer.Get(),
                tempBuffer.Size(),
                recvSize,
                priority,
                timeout.GetNextTimeout());
      }

      if ( successful && priority == IpcSysMessage::SYS_MSG_PRIORITY )
      {
        IpcSysMessage msg;
        msg.Decode(tempBuffer.Get());

        if ( msg.GetMessageType() == IpcSysMessage::MSG_PING )
        {
  #ifdef DEBUG_IPC
          DebugMessage("SyncIpcCommandReceive: Ping message received.\n");
  #endif
          continue;
        }
      }

      if ( successful )
      {
        // Successful receive, copy to buffer and post callback with no error

        dataBuffer->Resize(recvSize);
        memcpy(dataBuffer->Get(), tempBuffer.Get(), recvSize);

        break;
      }

      if ( !successful && timeout.Elapsed() )
      {
        // Our overall timeout period elapsed
        return false;
      }
    }
    catch(boost::interprocess::interprocess_exception &ex)
    {
      // Some kind of error
      throw CommonException(Error(
        boost::system::error_code(ex.get_native_error(), boost::system::get_system_category()),
        "SyncIpcCommandReceive(): Message queue receive call failed."));
    }
  }

  return true;
}

bool SyncIpcCommandReceive(
    MessageQueuePointer recvMessageQueue,
    MessageQueuePointer sendMessageQueue,
    boost::mutex & recvMutex,
    boost::mutex & sendMutex,
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
    if (! Receive(recvMessageQueue, sendMessageQueue, recvMutex, sendMutex, dataBuffer, priority, maxMilliseconds))
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
