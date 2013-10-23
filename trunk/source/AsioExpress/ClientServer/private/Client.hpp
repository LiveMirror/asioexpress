//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/ClientServer/ClientEventHandler.hpp"
#include "AsioExpress/ClientServer/ClientMessage.hpp"
#include "AsioExpress/ClientServer/private/IClientEvents.hpp"
#include "AsioExpress/ClientServer/private/MessagePortManager.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePort>
class Client : private AsioExpress::Coroutine
{
public:
  typedef typename MessagePort::EndPointType EndPointType; 
  typedef MessagePortManager<MessagePort> MessagePortManagerType;
  typedef boost::shared_ptr<MessagePortManagerType> MessagePortManagerPointer;

  Client(
      boost::asio::io_service & ioService,
      ClientInterfacePointer messagePortClient,
      IClientEventsPointer clientEvents,
      EndPointType endPoint,
      MessagePortManagerPointer messagePortManager);

  void operator()(
      AsioExpress::Error error = AsioExpress::Error());

private:
  typedef boost::shared_ptr<MessagePort> MessagePortPointer;
  typedef boost::shared_ptr<ClientEventHandler> ClientEventHandlerPointer; 
  typedef boost::shared_ptr<bool> BoolPointer;

  Client & operator=(Client const &);
  void Disconnect(AsioExpress::Error error);

  boost::asio::io_service &           m_ioService;
  ClientInterfacePointer              m_messagePortClient;
  IClientEventsPointer                m_clientEvents;
  EndPointType                        m_endPoint;
  MessagePortManagerPointer           m_messagePortManager;
  MessagePortPointer                  m_messagePort;
  MessagePortId                       m_messagePortId;
  DataBufferPointer                   m_buffer;
  BoolPointer                         m_useErrorHandler;
};

template<typename MessagePort>
Client<MessagePort>::Client(
    boost::asio::io_service & ioService,
    ClientInterfacePointer messagePortClient,
    IClientEventsPointer clientEvents,
    EndPointType endPoint,
    MessagePortManagerPointer messagePortManager) :
  m_ioService(ioService),
  m_messagePortClient(messagePortClient),
  m_clientEvents(clientEvents),
  m_endPoint(endPoint),
  m_messagePortManager(messagePortManager),
  m_messagePortId(0),
  m_buffer(new DataBuffer),
  m_useErrorHandler(new bool(false))
{
}

template<typename MessagePort>
void Client<MessagePort>::operator()(AsioExpress::Error error)
{
  if (*m_useErrorHandler && error)
  {
    *m_useErrorHandler = false;

    error = m_clientEvents->HandleMessageError(
              ClientMessage(
                  ClientConnection(
                    m_ioService,
                    m_messagePortId, 
                    m_messagePortClient), 
                  m_buffer, 
                  0),
              error);
  }

  if (error)
  {
    Disconnect(error);
    return;
  }

  REENTER (this)
  {
    // Create a new socket for the next incoming connection.
    m_messagePort.reset(new MessagePort(m_ioService));

    YIELD m_messagePort->AsyncConnect(m_endPoint, *this);

    m_messagePort->SetMessagePortOptions();

    m_messagePortId = m_messagePortManager->Add(m_messagePort);

    error = m_clientEvents->HandleConnected(
      ClientConnection(m_ioService, m_messagePortId, m_messagePortClient));
    if (error)
    {
      Disconnect(error);
      return;
    }
    
    // Process incoming messages.
    for(;;)
    {
      // Receive message
      YIELD
        m_messagePort->AsyncReceive(m_buffer, *this);

      YIELD 
      {
        *m_useErrorHandler = true;

        m_clientEvents->HandleMessage(
          ClientMessage(
              ClientConnection(
                m_ioService,
                m_messagePortId, 
                m_messagePortClient), 
              m_buffer, 
              *this));
      }
    }
  }
}

template<typename MessagePort>
void Client<MessagePort>::Disconnect(AsioExpress::Error error)
{
  m_messagePortManager->Remove(m_messagePortId);
  m_messagePort.reset();
  m_clientEvents->HandleDisconnected(
    ClientConnection(m_ioService, m_messagePortId, m_messagePortClient),
    error);
}

} // namespace MessagePort
} // namespace AsioExpress
