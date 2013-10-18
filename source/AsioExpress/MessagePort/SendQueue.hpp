//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <deque>

#include <boost/shared_ptr.hpp>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"

namespace AsioExpress {
namespace MessagePort {

class SendQueue
{
public:
  struct Item
  {
    Item(
        AsioExpress::MessagePort::DataBufferPointer dataBuffer,
        AsioExpress::CompletionHandler completionHandler) :
      dataBuffer(dataBuffer),
      completionHandler(completionHandler)
    {
    }

    AsioExpress::MessagePort::DataBufferPointer    dataBuffer;
    AsioExpress::CompletionHandler           completionHandler;
  };

  bool Empty() const
  {
    return m_queue.empty();
  }

  void Push(Item item)
  {
    m_queue.push_back(item);
  }

  Item Top() const
  {
    return m_queue.front();
  }

  void Pop()
  {
    m_queue.pop_front();
  }

  void Error(boost::asio::io_service& ioService, AsioExpress::Error error)
  {
    Queue::iterator  it = m_queue.begin();
    Queue::iterator end = m_queue.end();
    for (; it != end; ++it)
    {
      ioService.post(boost::asio::detail::bind_handler(it->completionHandler, error));
    }
    m_queue.clear();
  }

private:
  typedef std::deque<Item> Queue;

  Queue m_queue;
};

typedef boost::shared_ptr<SendQueue> SendQueuePointer;

} // namespace MessagePort
} // namespace AsioExpress
