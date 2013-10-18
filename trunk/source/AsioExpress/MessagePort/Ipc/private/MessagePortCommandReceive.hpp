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

class MessagePortCommandReceiveThread;

class MessagePortCommandReceive : private AsioExpress::Coroutine
{
public:
  inline MessagePortCommandReceive(boost::asio::io_service & ioService,
                                   ReceiveThreadPointer receiveThread, 
                                   MessageQueuePointer messageQueue, 
                                   DataBufferPointer dataBuffer, 
                                   AsioExpress::CompletionHandler completionHandler, 
                                   int maxMilliseconds = 0)
    : m_ioService(ioService),
      m_receiveThread(receiveThread),
      m_messageQueue(messageQueue),
      m_dataBuffer(dataBuffer),
      m_priority(new unsigned int(0)),
      m_completionHandler(completionHandler),
      m_maxMilliseconds(maxMilliseconds),
      m_work(ioService)
  { }

  void operator() (AsioExpress::Error e = AsioExpress::Error());

private:
  MessagePortCommandReceive operator=(MessagePortCommandReceive const &);

  boost::asio::io_service &         m_ioService;
  ReceiveThreadPointer              m_receiveThread;
  MessageQueuePointer               m_messageQueue;   
  DataBufferPointer                 m_dataBuffer;
  boost::shared_ptr<unsigned int>   m_priority;
  AsioExpress::CompletionHandler   m_completionHandler;
  int                               m_maxMilliseconds;
  boost::asio::io_service::work     m_work;
};


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
