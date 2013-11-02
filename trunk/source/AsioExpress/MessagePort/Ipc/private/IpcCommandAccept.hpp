//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePort.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePortAcceptor.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class MessagePortCommandReceive;

class IpcCommandAccept : private AsioExpress::Coroutine
{
public:
  inline IpcCommandAccept(
      MessagePortAcceptor & acceptor, 
      MessagePort & messagePort,
      AsioExpress::CompletionHandler completionHandler) :
    m_acceptor(acceptor),
    m_messagePort(messagePort),
    m_completionHandler(completionHandler),
    m_tempBuffer(new DataBuffer(1024)),
    m_work(acceptor.m_ioService)
  {
  }

  void operator() (AsioExpress::Error e = AsioExpress::Error());

private:
  IpcCommandAccept & operator=(IpcCommandAccept const &);

  void CallCompletionHandler(
    boost::system::error_code errorCode,
    std::string message);

  void CallCompletionHandler(
    AsioExpress::Error err);

  void WorkTimerExpired(boost::system::error_code errorCode);

  MessagePortAcceptor&                      m_acceptor;   
  MessagePort&                              m_messagePort;
  AsioExpress::CompletionHandler            m_completionHandler;
  DataBufferPointer                         m_tempBuffer;
  boost::asio::io_service::work             m_work;
  static int                                m_instance;
};

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
