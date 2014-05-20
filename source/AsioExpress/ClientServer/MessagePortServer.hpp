//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/ClientServer/private/InternalMessagePortServer.hpp"

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePortAcceptor>
class MessagePortServer : public ServerInterface
{
public:
  typedef typename MessagePortAcceptor::EndPointType EndPointType;

  MessagePortServer(
    boost::asio::io_service& ioService,
    EndPointType endPoint,
    ServerEventHandler * eventHandler);

  ~MessagePortServer();

  virtual void Start();

  virtual void Stop();

  virtual void GetIds(MessagePortIdList & list) const;

  virtual void AsyncSend(
      MessagePortId id, 
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

  virtual void AsyncBroadcast(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);

  virtual std::string GetAddress(MessagePortId id) const;

private:
  typedef InternalMessagePortServer<MessagePortAcceptor> ImplementationType;
  typedef boost::shared_ptr<ImplementationType> ImplementationPointer;

  ImplementationPointer  m_implementation;
};

template<typename MessagePortAcceptor>
MessagePortServer<MessagePortAcceptor>::MessagePortServer(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ServerEventHandler * eventHandler) :
  m_implementation(new InternalMessagePortServer<MessagePortAcceptor>(
    ioService, 
    endPoint, 
    eventHandler))
{
}

template<typename MessagePortAcceptor>
MessagePortServer<MessagePortAcceptor>::~MessagePortServer()
{
  m_implementation->Stop();
}

template<typename MessagePortAcceptor>
void MessagePortServer<MessagePortAcceptor>::Start()
{
  m_implementation->Start();
}

template<typename MessagePortAcceptor>
void MessagePortServer<MessagePortAcceptor>::Stop()
{
  m_implementation->Stop();
}

template<typename MessagePortAcceptor>
void MessagePortServer<MessagePortAcceptor>::GetIds(
    MessagePortIdList & list) const
{
  m_implementation->GetIds(list);
}

template<typename MessagePortAcceptor>
void MessagePortServer<MessagePortAcceptor>::AsyncSend(
    MessagePortId id, 
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_implementation->AsyncSend(id, buffer, completionHandler);
}

template<typename MessagePortAcceptor>
void MessagePortServer<MessagePortAcceptor>::AsyncBroadcast(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_implementation->AsyncBroadcast(buffer, completionHandler);
}

template<typename MessagePortAcceptor>
std::string MessagePortServer<MessagePortAcceptor>::GetAddress(
    MessagePortId id) const
{
    return m_implementation->GetAddress(id);
}

} // namespace MessagePort
} // namespace AsioExpress
