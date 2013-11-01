//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "AsioExpress/MessagePort/Ipc/IpcMessagePort.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/Ipc/private/MessageQueuePointer.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcReceiveThread.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class IpcMessagePortAcceptor
{
private:
  friend class IpcCommandAccept;

public:
  typedef IpcMessagePort MessagePortType;
  typedef IpcEndPoint EndPointType;

public:
  IpcMessagePortAcceptor(
    boost::asio::io_service & ioService,
    IpcEndPoint endPoint);

  ~IpcMessagePortAcceptor();

  void AsyncAccept(
      IpcMessagePort & messagePort, 
      AsioExpress::CompletionHandler completionHandler);

  void Close();

private:
  IpcMessagePortAcceptor & operator=(IpcMessagePortAcceptor const &);
  inline static std::string IntToString(int n)   { std::stringstream ss; std::string str; ss << n; ss >> str; return str; }

private:
  boost::asio::io_service &               m_ioService;
  IpcEndPoint                             m_endPoint;
  MessageQueuePointer                     m_messageQueue;
  IpcReceiveThreadPointer                 m_receiveThread;
};


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
