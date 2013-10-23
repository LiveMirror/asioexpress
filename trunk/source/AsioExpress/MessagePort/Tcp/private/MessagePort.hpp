//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <boost/asio.hpp>

#include "AsioExpressError/EcToErrorAdapter.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/MessagePort/SendQueue.hpp"
#include "AsioExpress/MessagePort/Tcp/TcpErrorCodes.hpp"
#include "AsioExpress/MessagePort/Tcp/EndPoint.hpp"
#include "AsioExpress/MessagePort/Tcp/private/SocketPointer.hpp"
#include "AsioExpress/MessagePort/Tcp/private/SetSocketOptions.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

template<typename H, typename ProtocolSender>
class AsyncSendHandler
{
public:
  typedef boost::shared_ptr<bool> BoolPointer;

  AsyncSendHandler(
      SocketPointer socket,
      BoolPointer isSending,
      AsioExpress::MessagePort::SendQueuePointer sendQueue,
      H completionHandler) :
    m_socket(socket),
    m_isSending(isSending),
    m_sendQueue(sendQueue),
    m_completionHandler(completionHandler)    
  {
  }

  void operator()(boost::system::error_code ec = boost::system::error_code())
  {
    if (ec)
    {
      // We close the socket if we get any error back.
      m_socket->close();

      if (! m_sendQueue->Empty())
        m_sendQueue->Error(m_socket->get_io_service(), AsioExpress::Error(ec));
    }

    if (! m_sendQueue->Empty())
    {
      AsioExpress::MessagePort::SendQueue::Item item = m_sendQueue->Top();
      m_sendQueue->Pop();

      ProtocolSender sender;
      sender.AsyncRun(
        m_socket, 
        item.dataBuffer, 
        AsyncSendHandler<AsioExpress::CompletionHandler, ProtocolSender>(
          m_socket, 
          m_isSending, 
          m_sendQueue, 
          item.completionHandler));      
    }
    else
    {
      *m_isSending = false;
    }

    m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, AsioExpress::Error(ec)));
  }

private:
   SocketPointer      m_socket;
   BoolPointer        m_isSending;      
   SendQueuePointer   m_sendQueue;
   H                  m_completionHandler;
};

template<typename ProtocolSender, typename ProtocolReceiver>
class MessagePort
{
public:
  typedef EndPoint EndPointType;

  MessagePort(boost::asio::io_service & ioService);

  ~MessagePort();

  SocketPointer GetSocket() const;

  void SetMessagePortOptions();
    
  template<typename H>
  void AsyncConnect(
      EndPointType endPoint, 
      H completionHandler);

  template<typename H>
  void AsyncSend(
      AsioExpress::MessagePort::DataBufferPointer buffer, 
      H completionHandler);

  template<typename H>
  void AsyncReceive(
      AsioExpress::MessagePort::DataBufferPointer buffer, 
      H completionHandler);

  void Disconnect();

private:
   typedef boost::shared_ptr<bool> BoolPointer;

   SocketPointer      m_socket;
   BoolPointer        m_isSending;      
   SendQueuePointer   m_sendQueue;
};

template<typename ProtocolSender, typename ProtocolReceiver>
MessagePort<ProtocolSender, ProtocolReceiver>::MessagePort(
    boost::asio::io_service & ioService) :
  m_socket(new boost::asio::ip::tcp::socket(ioService)),
  m_isSending(new bool(false)),
  m_sendQueue(new SendQueue)
{
}

template<typename ProtocolSender, typename ProtocolReceiver>
MessagePort<ProtocolSender, ProtocolReceiver>::~MessagePort()
{
  Disconnect();
}

template<typename ProtocolSender, typename ProtocolReceiver>
SocketPointer MessagePort<ProtocolSender, ProtocolReceiver>::GetSocket() const
{
  return m_socket;
}

template<typename ProtocolSender, typename ProtocolReceiver>
void MessagePort<ProtocolSender, ProtocolReceiver>::SetMessagePortOptions()
{
  SetSocketOptions(m_socket);
}

template<typename ProtocolSender, typename ProtocolReceiver>
template<typename H>
void MessagePort<ProtocolSender, ProtocolReceiver>::AsyncConnect(
    EndPointType endPoint, 
    H completionHandler)
{
  m_socket->async_connect(
    endPoint.GetEndPoint(m_socket->get_io_service()), 
    AsioExpress::EcToErrorAdapter<H>(completionHandler));
}

template<typename ProtocolSender, typename ProtocolReceiver>
template<typename H>
void MessagePort<ProtocolSender, ProtocolReceiver>::AsyncSend(
    AsioExpress::MessagePort::DataBufferPointer buffer,
    H completionHandler)
{
  if (*m_isSending)
  {
    m_sendQueue->Push(
      AsioExpress::MessagePort::SendQueue::Item(buffer, completionHandler));
    return;
  }

  *m_isSending = true;

  ProtocolSender sender;
  sender.AsyncRun(
    m_socket, 
    buffer, 
    AsyncSendHandler<H,ProtocolSender>(
      m_socket, 
      m_isSending, 
      m_sendQueue, 
      completionHandler));
}

template<typename ProtocolSender, typename ProtocolReceiver>
template<typename H>
void MessagePort<ProtocolSender, ProtocolReceiver>::AsyncReceive(
    AsioExpress::MessagePort::DataBufferPointer buffer,
    H completionHandler)
{
  ProtocolReceiver receiver;
  receiver.AsyncRun(
    m_socket, 
    buffer, 
    AsioExpress::EcToErrorAdapter<H>(completionHandler));
}

template<typename ProtocolSender, typename ProtocolReceiver>
void MessagePort<ProtocolSender, ProtocolReceiver>::Disconnect()
{
  m_socket->close();
}

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
