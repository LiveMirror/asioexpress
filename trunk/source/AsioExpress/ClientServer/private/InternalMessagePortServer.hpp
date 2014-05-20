//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "AsioExpress/MessagePort/DataBuffer.hpp"

#include "AsioExpress/ClientServer/ServerEventHandler.hpp"
#include "AsioExpress/ClientServer/ClientInterface.hpp"
#include "AsioExpress/ClientServer/private/IServerEvents.hpp"
#include "AsioExpress/ClientServer/private/MessagePortManager.hpp"
#include "AsioExpress/ClientServer/private/Server.hpp"
#include "AsioExpress/ClientServer/private/ServerEvents.hpp"
#include "AsioExpress/ClientServer/private/BroadcastProcessor.hpp"

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePortAcceptor>
class InternalMessagePortServer : 
  public ServerInterface, 
  public boost::enable_shared_from_this<InternalMessagePortServer<MessagePortAcceptor> >
{
public:
  typedef typename MessagePortAcceptor::EndPointType EndPointType;

  InternalMessagePortServer(
    boost::asio::io_service& ioService,
    EndPointType endPoint,
    ServerEventHandler * eventHandler);

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
  typedef boost::shared_ptr<MessagePortAcceptor> MessagePortAcceptorPointer;
  typedef typename MessagePortAcceptor::MessagePortType MessagePortType;
  typedef MessagePortManager<MessagePortType> MessagePortManagerType;
  typedef boost::shared_ptr<MessagePortManagerType> MessagePortManagerPointer;
  typedef Server<MessagePortAcceptor> ServerType;

  InternalMessagePortServer & operator=(InternalMessagePortServer const &);

  boost::asio::io_service &           m_ioService;
  EndPointType                        m_endPoint;
  MessagePortManagerPointer           m_messagePortManager;
  MessagePortAcceptorPointer          m_acceptor;
  IServerEventsPointer  m_serverEvents;
};

WIN_DISABLE_WARNINGS_BEGIN(4355)
template<typename MessagePortAcceptor>
InternalMessagePortServer<MessagePortAcceptor>::InternalMessagePortServer(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ServerEventHandler * eventHandler) :
  m_ioService(ioService),
  m_endPoint(endPoint),
  m_messagePortManager(new MessagePortManagerType(ioService)),
  m_serverEvents(new ServerEvents(eventHandler))
{
}
WIN_DISABLE_WARNINGS_END

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::Start()
{
  m_acceptor.reset(new MessagePortAcceptor(m_ioService, m_endPoint));

  ServerType server(
    m_ioService,
    this->shared_from_this(),
    m_serverEvents,
    m_acceptor, 
    m_messagePortManager);

  server();
}

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::Stop()
{
  m_messagePortManager->RemoveAll();
  if (m_acceptor) 
  {
    m_acceptor->Close();
    m_acceptor.reset();
  }
}

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::GetIds(
    MessagePortIdList & list) const
{
  m_messagePortManager->GetIds(list);
}

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::AsyncSend(
    MessagePortId id, 
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  m_messagePortManager->AsyncSend(id, buffer, completionHandler);
}

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::AsyncBroadcast(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
  MessagePortIdList idList;
  m_messagePortManager->GetIds(idList);
  BroadcastProcessor proc(
    m_ioService,
    m_messagePortManager, 
    idList, 
    buffer, 
    completionHandler);
  proc();
}

template<typename MessagePortAcceptor>
std::string InternalMessagePortServer<MessagePortAcceptor>::GetAddress(
    MessagePortId id) const
{
    return m_messagePortManager->GetAddress(id);
}

} // namespace MessagePort
} // namespace AsioExpress
