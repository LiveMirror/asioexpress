//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#pragma warning(disable: 4505) // "unreference local function removed" bug with templates

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
namespace ClientServer {

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

private:
  typedef boost::shared_ptr<MessagePortAcceptor> MessagePortAcceptorPointer;
  typedef typename MessagePortAcceptor::MessagePortType MessagePortType;
  typedef ClientServer::MessagePortManager<MessagePortType> MessagePortManagerType;
  typedef boost::shared_ptr<MessagePortManagerType> MessagePortManagerPointer;
  typedef ClientServer::Server<MessagePortAcceptor> ServerType;

  InternalMessagePortServer & operator=(InternalMessagePortServer const &);

  boost::asio::io_service &           m_ioService;
  EndPointType                        m_endPoint;
  MessagePortManagerPointer           m_messagePortManager;
  MessagePortAcceptorPointer          m_acceptor;
  ClientServer::IServerEventsPointer  m_serverEvents;
};

#pragma warning(push)
#pragma warning(disable: 4355)
template<typename MessagePortAcceptor>
InternalMessagePortServer<MessagePortAcceptor>::InternalMessagePortServer(
    boost::asio::io_service & ioService,
    EndPointType endPoint,
    ServerEventHandler * eventHandler) :
  m_ioService(ioService),
  m_endPoint(endPoint),
  m_messagePortManager(new MessagePortManagerType(ioService)),
  m_serverEvents(new ClientServer::ServerEvents(eventHandler))
{
}
#pragma warning(pop)

template<typename MessagePortAcceptor>
void InternalMessagePortServer<MessagePortAcceptor>::Start()
{
  m_acceptor.reset(new MessagePortAcceptor(m_ioService, m_endPoint));

  ServerType server(
    m_ioService,
    shared_from_this(),
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
  ClientServer::BroadcastProcessor proc(
    m_ioService,
    m_messagePortManager, 
    idList, 
    buffer, 
    completionHandler);
  proc();
}

} // namespace ClientServer
} // namespace MessagePort
} // namespace AsioExpress
