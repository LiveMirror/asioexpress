//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/ClientServer/private/InternalMessagePortClient.hpp"

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePort>
class MessagePortClient : public ClientInterface
{
public:
  typedef typename MessagePort::EndPointType EndPointType;

  MessagePortClient(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ClientEventHandler * eventHandler);

  ~MessagePortClient();

  virtual void Connect();

  virtual void ShutDown();

  virtual void Disconnect();

  virtual void AsyncSend(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);
 
private:
  typedef InternalMessagePortClient<MessagePort> ImplementationType;
  typedef boost::shared_ptr<ImplementationType> ImplementationPointer;

  ImplementationPointer  m_implementation;
};

template<typename MessagePort>
MessagePortClient<MessagePort>::MessagePortClient(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ClientEventHandler * eventHandler) :
  m_implementation(new InternalMessagePortClient<MessagePort>(
    ioService, 
    endPoint, 
    eventHandler))
{
}

template<typename MessagePort>
MessagePortClient<MessagePort>::~MessagePortClient()
{
  m_implementation->Disconnect();
}

template<typename MessagePort>
void MessagePortClient<MessagePort>::Connect()
{
  m_implementation->Connect();
}

template<typename MessagePort>
void MessagePortClient<MessagePort>::Disconnect()
{
  m_implementation->Disconnect();
}

template<typename MessagePort>
void MessagePortClient<MessagePort>::ShutDown()
{
  m_implementation->ShutDown();
}

template<typename MessagePort>
void MessagePortClient<MessagePort>::AsyncSend(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
    m_implementation->AsyncSend(buffer, completionHandler);
}

} // namespace MessagePort
} // namespace AsioExpress
