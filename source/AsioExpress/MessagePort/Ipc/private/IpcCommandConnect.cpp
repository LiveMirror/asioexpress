//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include <boost/random.hpp>

#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandConnect.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSysMessage.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandReceive.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.
#include "AsioExpress/Platform/DebugMessage.hpp"

namespace
{
  boost::mt19937 rng(static_cast<boost::uint32_t>(std::time(0)));
}

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

void IpcCommandConnect::operator() (AsioExpress::Error e)
{
  if (e)
  {
    m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, e));
    return;
  }

  REENTER(this)
  {
    // Need to start a block here to avoid errors like:
    // C2360: initialization of 'serverQueueName' is skipped by 'case' label
    {    
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Finding new message queue ID.\n");
#endif

      m_messagePort.Disconnect();

      //
      // Step 1 - Find a valid message queue name that is not in use
      //

      std::string clientQueueName;
      std::string serverQueueName;
      bool finished = false;

      while ( !finished )
      {
        boost::uniform_int<> connectionIdRange(LowConnectionId, HighConnectionId);
        boost::variate_generator<boost::mt19937&, boost::uniform_int<> > randomNumber(rng, connectionIdRange);
        std::string id = IntToString(randomNumber());

        clientQueueName = m_endPoint.GetEndPoint() + "#Client#" + id;
        serverQueueName = m_endPoint.GetEndPoint() + "#Server#" + id;

        // Check that the queue doesn't already exist

        try {
          boost::interprocess::message_queue mq1(boost::interprocess::open_only, clientQueueName.c_str());
          // No exception thrown - this queue already exists. Try again.
          continue;
        }
        catch(boost::interprocess::interprocess_exception&) {
          // Couldn't open queue, so it doesn't exist. We can proceed.
        }

        try {
          boost::interprocess::message_queue mq1(boost::interprocess::open_only, serverQueueName.c_str());
          // No exception thrown - this queue already exists. Try again.
          continue;
        }
        catch(boost::interprocess::interprocess_exception&) {
          // Couldn't open queue, so it doesn't exist. We can proceed.
          finished = true;
        }
      }

      //
      // Step 2 - Create the message queues for client & server
      //
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Create new message queues.\n");
#endif

      try {
        m_messagePort.m_recvMessageQueueName = clientQueueName;
        m_messagePort.m_sendMessageQueueName = serverQueueName;
        m_messagePort.m_recvMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::create_only, m_messagePort.m_recvMessageQueueName.c_str(), m_endPoint.GetMaxNumMsg(), m_endPoint.GetMaxMsgSize()));
        m_messagePort.m_sendMessageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::create_only, m_messagePort.m_sendMessageQueueName.c_str(), m_endPoint.GetMaxNumMsg(), m_endPoint.GetMaxMsgSize()));
        m_messagePort.m_receiveThread.reset(new IpcReceiveThread(m_messagePort.m_ioService, m_messagePort.m_recvMessageQueue, IpcReceiveThread::EnablePing));
        m_messagePort.m_sendThread.reset(new IpcSendThread(m_messagePort.m_ioService, m_messagePort.m_sendMessageQueue, IpcSendThread::EnablePing));
      }
      catch(boost::interprocess::interprocess_exception& ex) 
      {
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Unable to create client/server message queues!\n");
#endif
        m_messagePort.Disconnect();
        AsioExpress::Error err(
          boost::system::error_code(ex.get_native_error(), boost::system::get_system_category()),
          "MessagePort::AsyncConnect(): Unable to create client/server message queues.");
        m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, err));
        return;
      }

      //
      // Step 3 - Try to connect to the server's acceptor message queue to
      //          notify it we want to connect & pass through the queue names.
      //

      try {
        boost::interprocess::message_queue acceptor(boost::interprocess::open_only, m_endPoint.GetEndPoint().c_str());

        IpcSysMessage msg(IpcSysMessage::MSG_CONNECT);
        msg.AddParam(m_messagePort.m_recvMessageQueueName);
        msg.AddParam(m_messagePort.m_sendMessageQueueName);

        char buf[1024];
        int len = msg.Encode(buf);

#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Sending connect message.\n");
#endif
        if ( !acceptor.try_send(buf, len, IpcSysMessage::SYS_MSG_PRIORITY) )
        {
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Error sending connect message!\n");
#endif
          m_messagePort.Disconnect();
          AsioExpress::Error err(
            ErrorCode::CommunicationFailure,
            "MessagePort::AsyncConnect(): Server's acceptor queue is full.");
          m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, err));
          return;
        }
      }
      catch(boost::interprocess::interprocess_exception &ex) {    
        m_messagePort.Disconnect();
        AsioExpress::Error err(
          boost::system::error_code(ex.get_native_error(), boost::system::get_system_category()),
          "MessagePort::AsyncConnect(): Unable to open server acceptor queue.");
        m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, err));
        return;
      }
    }

    //
    // Step 4 - Asynchronously wait for a connection acknowledgement system message
    //          from the server to our receive message queue
    //

#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Waiting for ACK message.\n");
#endif

    YIELD 
      IpcCommandReceive(m_messagePort.m_ioService,
                        m_messagePort.m_receiveThread,
                        m_messagePort.m_recvMessageQueue,
                        m_dataBuffer,
                        *this,
                        8000)();

    //
    // Step 5 - Validate the connection acknowledgement
    //

    IpcSysMessage msg2;
    msg2.Decode(m_dataBuffer->Get());
    
    if ( msg2.GetMessageType() != IpcSysMessage::MSG_CONNECT_ACK )
    {
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Invalid CONNECT-ACK response from server!\n");
#endif
      m_messagePort.Disconnect();
      AsioExpress::Error err(
        ErrorCode::CommunicationFailure,
        "MessagePort::AsyncConnect(): Invalid CONNECT-ACK response from server.");
      m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, err));
      return;
    }

    // Success
#ifdef DEBUG_IPC
      DebugMessage("IpcCommandConnect: Connected.\n");
#endif

    m_messagePort.m_ioService.post(boost::asio::detail::bind_handler(m_completionHandler, AsioExpress::Error()));
  }
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
