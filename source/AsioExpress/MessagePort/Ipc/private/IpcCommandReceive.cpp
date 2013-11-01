//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include "AsioExpress/MessagePort/Ipc/IpcErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandReceive.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.
#include "AsioExpress/Platform/DebugMessage.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

void IpcCommandReceive::operator() (AsioExpress::Error e)
{
  if (e)
  {
    m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, e));
    return;
  }

  REENTER(this)
  {
    // 
    // Step 1 - Create a temporary MessagePortCommandReceiveThread instance to
    //          do the actual receive in a new thread.
    //
       
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandReceive: Waiting to receive message.\n");
#endif

    YIELD 
      m_receiveThread->AsyncReceive(
        m_dataBuffer, 
        m_priority,
        *this, 
        m_maxMilliseconds);

    //
    // Step 2 - If this is a standard message port, check for a disconnect message
    //

    if ( *m_priority == IpcSysMessage::SYS_MSG_PRIORITY )
    {
      IpcSysMessage msg;
      msg.Decode(m_dataBuffer->Get());

      if ( msg.GetMessageType() == IpcSysMessage::MSG_DISCONNECT )
      {
        // This is a disconnect message; return an error.

#ifdef DEBUG_IPC
      DebugMessage("IpcCommandReceive: A disconnect message was received.\n");
#endif

        AsioExpress::Error err(
          ErrorCode::Disconnected,
          "MessagePort::AsyncReceive(): Connection was disconnected by peer.");
        m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, err));
        return;
      }
    }

    // 
    // Step 3 - Receive was successful, invoke the callback.
    //
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandReceive: Message received.\n");
#endif

    m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, AsioExpress::Error()));
  }
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
