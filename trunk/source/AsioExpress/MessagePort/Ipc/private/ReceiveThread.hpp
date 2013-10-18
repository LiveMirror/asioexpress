//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/thread.hpp>

#include "AsioExpress/MessagePort/Ipc/private/MessageQueuePointer.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class ReceiveThread
{
public:
  ReceiveThread(
      boost::asio::io_service & ioService,
      MessageQueuePointer messageQueue);

  ~ReceiveThread();

  void AsyncReceive(
      DataBufferPointer dataBuffer, 
      boost::shared_ptr<unsigned int> priority,
      AsioExpress::CompletionHandler completionHandler, 
      int maxMilliseconds = 0);

  void CancelReceive();

  void Close();

private:
  ReceiveThread(ReceiveThread const & );
  ReceiveThread & operator=(ReceiveThread const &);

  struct ReceiveParameters
  {
    DataBufferPointer                 dataBuffer;
    boost::shared_ptr<unsigned int>   priority;
    AsioExpress::CompletionHandler   completionHandler;
    int                               maxMilliseconds;
  };

  void ReceiveFunction();

  void Receive();

  void CallCompletionHandler(
    boost::system::error_code errorCode,
    std::string message);

  void CallCompletionHandler(
    AsioExpress::Error err);

  boost::asio::io_service &   m_ioService;
  MessageQueuePointer         m_messageQueue;

  bool                        m_isReceiving; // only set by thread function
  bool                        m_isCanceled;  // only read by thread function
  bool                        m_isClosing;   // only read by thread function

  ReceiveParameters           m_parameters;

  boost::mutex                m_alertMutex;
  boost::condition_variable   m_alert;
  bool                        m_alertThrown;

  boost::thread               m_thread;
};

typedef boost::shared_ptr<ReceiveThread> ReceiveThreadPointer;

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
