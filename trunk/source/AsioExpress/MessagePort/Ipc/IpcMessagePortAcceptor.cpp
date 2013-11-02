//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePortAcceptor.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandAccept.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcCommandConnect.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {


MessagePortAcceptor::MessagePortAcceptor(
    boost::asio::io_service & ioService,
    EndPoint endPoint) :
  m_ioService(ioService),
  m_endPoint(endPoint)
{
  // Remove previous instance of acceptor queue
  boost::interprocess::message_queue::remove(endPoint.GetEndPoint().c_str());


  // Clear all client/server message queues for this endpoint to avoid stale queues
  // due to client crashes
  std::string name;
  for ( int i = IpcCommandConnect::LowConnectionId; 
        i <= IpcCommandConnect::HighConnectionId; 
        i++ )
  {
    name = m_endPoint.GetEndPoint() + "#Server#" + IntToString(i);
    boost::interprocess::message_queue::remove(name.c_str());
    name = m_endPoint.GetEndPoint() + "#Client#" + IntToString(i);
    boost::interprocess::message_queue::remove(name.c_str());
  }


  // Create our acceptor message queue
  m_messageQueue.reset(new boost::interprocess::message_queue(boost::interprocess::create_only, endPoint.GetEndPoint().c_str(), endPoint.GetMaxNumMsg(), endPoint.GetMaxMsgSize()));

  m_receiveThread.reset(new IpcReceiveThread(ioService, m_messageQueue));
}


MessagePortAcceptor::~MessagePortAcceptor()
{
  Close();
}

void MessagePortAcceptor::Close()
{
  // Before destroying the acceptor, make sure any receive thread is not running
  if (m_receiveThread)
    m_receiveThread->Close();

  // Ok now we can destroy the queue
  m_messageQueue.reset();
  boost::interprocess::message_queue::remove(m_endPoint.GetEndPoint().c_str());
}


void MessagePortAcceptor::AsyncAccept(
    MessagePort & messagePort, 
    AsioExpress::CompletionHandler completionHandler)
{
  IpcCommandAccept(*this, messagePort, completionHandler)();
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
