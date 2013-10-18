//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/Tcp/EndPoint.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

template<typename MessagePort>
class MessagePortAcceptor
{
public:
  typedef MessagePort MessagePortType;
  typedef EndPoint EndPointType;

  MessagePortAcceptor(
    boost::asio::io_service & ioService,
    EndPointType endPoint);

  ~MessagePortAcceptor();

  template<typename CompletionHandler>
  void AsyncAccept(
      MessagePort & messagePort, 
      CompletionHandler completionHandler);

  void Close();

private:
  typedef boost::shared_ptr<boost::asio::ip::tcp::acceptor> AcceptorPointer;

  AcceptorPointer m_acceptor;
};

template<typename MessagePort>
MessagePortAcceptor<MessagePort>::MessagePortAcceptor(
    boost::asio::io_service & ioService,
    EndPointType endPoint)
{
  m_acceptor.reset(
    new boost::asio::ip::tcp::acceptor(
      ioService, 
      endPoint.GetEndPoint(ioService)));
}

template<typename MessagePort>
MessagePortAcceptor<MessagePort>::~MessagePortAcceptor()
{
  Close();
}

template<typename MessagePort>
template<typename CompletionHandler>
void MessagePortAcceptor<MessagePort>::AsyncAccept(
    MessagePort & messagePort, 
    CompletionHandler completionHandler)
{
  m_acceptor->async_accept(
    *messagePort.GetSocket(), 
    AsioExpress::EcToErrorAdapter<CompletionHandler>(completionHandler));
}

template<typename MessagePort>
void MessagePortAcceptor<MessagePort>::Close()
{
  m_acceptor->close();
}

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
