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

class SendThread
{
public:
  SendThread(
      boost::asio::io_service & ioService,
      MessageQueuePointer messageQueue);

  ~SendThread();

  void AsyncSend(
      DataBufferPointer dataBuffer, 
      unsigned int priority,
      AsioExpress::CompletionHandler completionHandler);

  void Close();

private:
  SendThread(SendThread const & );
  SendThread & operator=(SendThread const &);

  struct SendParameters
  {
    DataBufferPointer                 dataBuffer;
    unsigned int                      priority;
    AsioExpress::CompletionHandler   completionHandler;
  };

  typedef std::vector<SendParameters> SendQueue;

  void SendFunction();

  void Send();

  void Send(SendParameters const & parameters);

  void CallCompletionHandlers(
    SendQueue::iterator parameters,
    SendQueue::iterator end,
    AsioExpress::Error error);

  void CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    boost::system::error_code errorCode,
    std::string message);

  void CallCompletionHandler(
    AsioExpress::CompletionHandler completionHandler,
    AsioExpress::Error error);

  boost::asio::io_service &   m_ioService;
  MessageQueuePointer         m_messageQueue;

  bool                        m_isClosing;   // only read by thread function
  bool                        m_sendFailed;

  SendQueue                   m_sendQueue;
  boost::mutex                m_sendQueueMutex;

  boost::mutex                m_alertMutex;
  boost::condition_variable   m_alert;
  bool                        m_alertThrown;

  boost::thread               m_thread;
};

typedef boost::shared_ptr<SendThread> SendThreadPointer;

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
