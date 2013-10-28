//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <map>
#include <boost/asio.hpp>

#include "AsioExpressError/Check.hpp"
#include "AsioExpress/ClientServer/MessagePortId.hpp"
#include "AsioExpress/ClientServer/private/IAsyncSend.hpp"

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePort>
class MessagePortManager : public IAsyncSend
{
public:
  typedef boost::shared_ptr<MessagePort> MessagePortPointer;

  MessagePortManager(boost::asio::io_service& ioService);
  
  virtual ~MessagePortManager() {};

  MessagePortId Add(MessagePortPointer messagePort);

  void Remove(MessagePortId id);

  void RemoveAll();

  void GetIds(MessagePortIdList & list) const;

  void AsyncSend(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

  virtual void AsyncSend(
      MessagePortId id, 
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

private:
  typedef std::map<MessagePortId, MessagePortPointer> MessagePortMap;

  boost::asio::io_service*  m_ioService;
  MessagePortId             m_currentId;
  MessagePortMap            m_messagePortMap;
};

template<typename MessagePort>
MessagePortManager<MessagePort>::MessagePortManager(boost::asio::io_service& ioService) :
  m_ioService(&ioService),
  m_currentId(0) 
{
}

template<typename MessagePort>
MessagePortId
MessagePortManager<MessagePort>::Add(MessagePortPointer messagePort)
{
  ++m_currentId;
  if (m_currentId < 0)
    m_currentId = 1;
  m_messagePortMap[m_currentId] = messagePort;
  return m_currentId;
}

template<typename MessagePort>
void MessagePortManager<MessagePort>::Remove(MessagePortId id)
{
  m_messagePortMap.erase(id);
}

template<typename MessagePort>
void MessagePortManager<MessagePort>::RemoveAll()
{
  typename MessagePortMap::iterator  it = m_messagePortMap.begin();
  typename MessagePortMap::iterator end = m_messagePortMap.end();
  for(; it != end; ++it)
  {
    it->second->Disconnect();
  }
  m_messagePortMap.clear();
}

template<typename MessagePort>
void MessagePortManager<MessagePort>::GetIds(MessagePortIdList & list) const
{
  typename MessagePortMap::const_iterator  it = m_messagePortMap.begin();
  typename MessagePortMap::const_iterator end = m_messagePortMap.end();

  for(; it != end; ++it)
  {
    list.push_back(it->first);
  }
}

template<typename MessagePort>
void MessagePortManager<MessagePort>::AsyncSend(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  if (m_messagePortMap.size() > 0)
  {
    CHECK(m_messagePortMap.size() == 1);  
    typename MessagePortMap::iterator messagePortIterator = m_messagePortMap.begin();
    messagePortIterator->second->AsyncSend(buffer, completionHandler);
  }
  else
  {
    // We don't consider this an error. The message port is allowed to be 
    // disconnected. The application will receive the disconnect notification.
    m_ioService->post(boost::asio::detail::bind_handler(completionHandler, AsioExpress::Error()));
  }
}

template<typename MessagePort>
void MessagePortManager<MessagePort>::AsyncSend(
    MessagePortId id, 
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  typename MessagePortMap::iterator messagePortIterator = m_messagePortMap.find(id);

  if (messagePortIterator != m_messagePortMap.end())
  {
    messagePortIterator->second->AsyncSend(buffer, completionHandler);
  }
  else
  {
    // We don't consider this an error. The message port is allowed to be 
    // disconnected. The application will receive the disconnect notification.
    m_ioService->post(boost::asio::detail::bind_handler(completionHandler, AsioExpress::Error()));
  }
}

} // namespace MessagePort
} // namespace AsioExpress
