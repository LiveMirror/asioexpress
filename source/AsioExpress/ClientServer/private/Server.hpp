//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/ClientServer/ServerMessage.hpp"
#include "AsioExpress/ClientServer/ServerEventHandler.hpp"
#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace MessagePort {

template<typename MessagePortAcceptor>
class Server : private AsioExpress::Coroutine
{
public:
  typedef typename MessagePortAcceptor::EndPointType EndPointType; 
  typedef MessagePortManager<typename MessagePortAcceptor::MessagePortType> MessagePortManagerType;
  typedef boost::shared_ptr<MessagePortManagerType> MessagePortManagerPointer;
  typedef boost::shared_ptr<MessagePortAcceptor> MessagePortAcceptorPointer;

  Server(
      boost::asio::io_service & ioService,
      ServerInterfacePointer messagePortServer,
      IServerEventsPointer serverEvents,
      MessagePortAcceptorPointer acceptor,
      MessagePortManagerPointer messagePortManager);
  
  void operator()(
      AsioExpress::Error error = AsioExpress::Error());

private:
  typedef typename MessagePortAcceptor::MessagePortType MessagePort;
  typedef boost::shared_ptr<MessagePort> MessagePortPointer;

  Server & operator=(Server const &);
  void Disconnect(AsioExpress::Error error);

  boost::asio::io_service &           m_ioService;
  ServerInterfacePointer              m_messagePortServer;
  IServerEventsPointer                m_serverEvents;
  MessagePortManagerPointer           m_messagePortManager;
  MessagePortAcceptorPointer          m_acceptor;
  MessagePortPointer                  m_messagePort;
  MessagePortId                       m_messagePortId;
  DataBufferPointer                   m_buffer;
};

template<typename MessagePortAcceptor>
Server<MessagePortAcceptor>::Server(
    boost::asio::io_service & ioService,
    ServerInterfacePointer messagePortServer,
    IServerEventsPointer serverEvents,
    MessagePortAcceptorPointer acceptor,
    MessagePortManagerPointer messagePortManager) :
  m_ioService(ioService),
  m_messagePortServer(messagePortServer),
  m_serverEvents(serverEvents),
  m_messagePortManager(messagePortManager),
  m_acceptor(acceptor),
  m_messagePortId(0)
{
}

template<typename MessagePortAcceptor>
void Server<MessagePortAcceptor>::operator()(
    AsioExpress::Error error)
{
  REENTER (this)
  {
    // Loop to accept incoming connections.
    do
    {
      // Create a new socket for the next incoming connection.
      m_messagePort.reset(new MessagePort(m_ioService));

      // Accept a new connection. The "YIELD" pseudo-keyword saves the current
      // line number and exits the coroutine's "REENTER" block. We use the
      // server coroutine as the completion handler for the async_accept
      // operation. When the asynchronous operation completes, the io_service
      // invokes the function call operator, we "REENTER" the coroutine, and
      // then control resumes at the following line.
      YIELD m_acceptor->AsyncAccept(*m_messagePort, *this);
      if (error.GetErrorCode() == boost::asio::error::operation_aborted ||
          error.GetErrorCode() == boost::asio::error::bad_descriptor)
      {
        return;
      }
        
      if (error)
      {
        AsioExpress::Error acceptorError(
          AsioExpress::ErrorCode::MessagePortAcceptorError,
          error.Message());
        m_serverEvents->HandleDisconnected(
          ServerConnection(m_ioService, 0, m_messagePortServer),
          acceptorError);
        continue;
      }

      // Sockets require us to do some initialization after the connect.
      m_messagePort->SetMessagePortOptions();

      // We "FORK" by cloning a new server coroutine to handle the connection.
      // After forking we have a parent coroutine and a child coroutine. Both
      // parent and child continue execution at the following line. They can
      // be distinguished using the functions coroutine::is_parent() and
      // coroutine::is_child().
      FORK Server(*this)();

      // The parent continues looping to accept the next incoming connection.
      // The child exits the loop and processes the connection.
    } while (IsParent());


    m_messagePortId = m_messagePortManager->Add(m_messagePort);

    error = m_serverEvents->HandleConnected(
      ServerConnection(m_ioService, m_messagePortId, m_messagePortServer));
    if (error)
    {
      Disconnect(error);
      return;
    }

    m_buffer.reset(new DataBuffer());

    // Process incoming messages.
    for(;;)
    {
      // Receive message
      YIELD 
        m_messagePort->AsyncReceive(m_buffer, *this);
      if (error)
      {
        Disconnect(error);
        return;
      }

      YIELD 
      {
        m_serverEvents->HandleMessage(
          ServerMessage(
            ServerConnection(
              m_ioService,
              m_messagePortId, 
              m_messagePortServer), 
            m_buffer, 
            *this));
      }
      if (error)
      {
        error = m_serverEvents->HandleMessageError(
                  ServerMessage(
                    ServerConnection(
                      m_ioService,
                      m_messagePortId, 
                      m_messagePortServer), 
                    m_buffer, 
                    0),
                  error);
        if (error)
        {
          Disconnect(error);
          return;
        }
      }
    }
  }
}

template<typename MessagePortAcceptor>
void Server<MessagePortAcceptor>::Disconnect(AsioExpress::Error error)
{
  m_messagePortManager->Remove(m_messagePortId);
  m_messagePort.reset();
  m_serverEvents->HandleDisconnected(
    ServerConnection(m_ioService, m_messagePortId, m_messagePortServer),
    error);
}

} // namespace MessagePort
} // namespace AsioExpress
