//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "AsioExpress/MessagePort/DataBuffer.hpp"

#include "AsioExpress/ClientServer/ClientEventHandler.hpp"
#include "AsioExpress/ClientServer/ClientInterface.hpp"
#include "AsioExpress/ClientServer/private/IClientEvents.hpp"
#include "AsioExpress/ClientServer/private/MessagePortManager.hpp"
#include "AsioExpress/ClientServer/private/Client.hpp"
#include "AsioExpress/ClientServer/private/ClientEvents.hpp"

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePort>
class InternalMessagePortClient : 
  public ClientInterface, 
  public boost::enable_shared_from_this<InternalMessagePortClient<MessagePort> >
{
public:
  typedef typename MessagePort::EndPointType EndPointType;

  InternalMessagePortClient(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ClientEventHandler * eventHandler);

//  virtual void Connect();
virtual void Connect()
{
  if (m_isShutDown)
    return;

  ClientType client(
    m_ioService, 
    this->shared_from_this(),
    m_clientEvents,
    m_endPoint, 
    m_messagePortManager);
  
  client();
}

  virtual void Disconnect();

  virtual void ShutDown();

  virtual void AsyncSend(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);
 
private:
  typedef MessagePortManager<MessagePort> MessagePortManagerType;
  typedef boost::shared_ptr<MessagePortManagerType> MessagePortManagerPointer;
  typedef Client<MessagePort> ClientType;

  InternalMessagePortClient & operator=(InternalMessagePortClient const &);

  void AsyncSendCompleted(AsioExpress::Error error);

  boost::asio::io_service &           m_ioService;
  EndPointType                        m_endPoint;
  MessagePortManagerPointer           m_messagePortManager;
  IClientEventsPointer                m_clientEvents;
  bool                                m_isShutDown;
};

WIN_DISABLE_WARNINGS_BEGIN(4355)
template<typename MessagePort>
InternalMessagePortClient<MessagePort>::InternalMessagePortClient(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ClientEventHandler * eventHandler) :
  m_ioService(ioService),
  m_endPoint(endPoint),
  m_messagePortManager(new MessagePortManagerType(ioService)),
  m_clientEvents(new ClientEvents(eventHandler)),
  m_isShutDown(false)
{
}
WIN_DISABLE_WARNINGS_END

//template<typename MessagePort>
//void InternalMessagePortClient<MessagePort>::Connect()
//{
//  if (m_isShutDown)
//    return;
//
//  ClientType client(
//    m_ioService, 
//    shared_from_this(),
//    m_clientEvents,
//    m_endPoint, 
//    m_messagePortManager);
//  
//  client();
//}

template<typename MessagePort>
void InternalMessagePortClient<MessagePort>::Disconnect()
{
  // Message port manager should only contain one connection.
  m_messagePortManager->RemoveAll();
}

template<typename MessagePort>
void InternalMessagePortClient<MessagePort>::ShutDown()
{
  Disconnect();
  m_isShutDown = true;
}

template<typename MessagePort>
void InternalMessagePortClient<MessagePort>::AsyncSend(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_messagePortManager->AsyncSend(buffer, completionHandler);
}

} // namespace MessagePort
} // namespace AsioExpress
