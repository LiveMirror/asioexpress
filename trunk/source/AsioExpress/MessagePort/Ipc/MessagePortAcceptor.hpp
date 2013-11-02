//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "AsioExpress/MessagePort/Ipc/MessagePort.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/Ipc/private/MessageQueuePointer.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcReceiveThread.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class MessagePortAcceptor
{
private:
  friend class IpcCommandAccept;

public:
  typedef MessagePort MessagePortType;
  typedef EndPoint EndPointType;

public:
  MessagePortAcceptor(
    boost::asio::io_service & ioService,
    EndPoint endPoint);

  ~MessagePortAcceptor();

  void AsyncAccept(
      MessagePort & messagePort, 
      AsioExpress::CompletionHandler completionHandler);

  void Close();

private:
  MessagePortAcceptor & operator=(MessagePortAcceptor const &);
  inline static std::string IntToString(int n)   { std::stringstream ss; std::string str; ss << n; ss >> str; return str; }

private:
  boost::asio::io_service &               m_ioService;
  EndPoint                                m_endPoint;
  MessageQueuePointer                     m_messageQueue;
  IpcReceiveThreadPointer                 m_receiveThread;
};


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
