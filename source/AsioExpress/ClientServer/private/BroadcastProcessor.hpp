//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/MessagePortId.hpp"
#include "AsioExpress/ClientServer/private/IAsyncSend.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace MessagePort {

class BroadcastProcessor : private AsioExpress::Coroutine
{
public:
  typedef std::vector<MessagePortId> MessagePortIdList;
  typedef boost::shared_ptr<MessagePortIdList> MessagePortIdListPointer;

  BroadcastProcessor(
      boost::asio::io_service& ioService,
      IAsyncSendPointer sender,
      MessagePortIdList const & messagePortIdList,
      DataBufferPointer buffer,
      AsioExpress::CompletionHandler completionHandler) :
    m_ioService(&ioService),
    m_sender(sender),
    m_messagePortIdList(new MessagePortIdList(messagePortIdList)),
    m_index(0),
    m_buffer(buffer),
    m_completionHandler(completionHandler)
  {
  }

  void operator()(AsioExpress::Error error = AsioExpress::Error())
  {
    // We will ignore any send errors but we will wait for all sends to
    // complete nonetheless.

    REENTER(this)
    {
      for(; m_index < m_messagePortIdList->size(); ++m_index)
      {
        YIELD
          m_sender->AsyncSend(m_messagePortIdList->at(m_index), m_buffer, *this);
      }

      CallCompletionHandler(*m_ioService, m_completionHandler, AsioExpress::Error());
      m_completionHandler = 0;
   }
  }

private:
    boost::asio::io_service*          m_ioService;
    IAsyncSendPointer                 m_sender;
    MessagePortIdListPointer          m_messagePortIdList;
    MessagePortIdList::size_type      m_index;
    DataBufferPointer                 m_buffer;
    AsioExpress::CompletionHandler    m_completionHandler;
};

} // namespace MessagePort
} // namespace AsioExpress
