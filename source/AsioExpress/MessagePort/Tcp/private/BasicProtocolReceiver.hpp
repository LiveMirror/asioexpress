//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/function.hpp>
#include <boost/asio.hpp>

#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/CompletionHandler.hpp"

#include "AsioExpress/MessagePort/Tcp/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/MessagePort/Tcp/private/SocketPointer.hpp"
#include "AsioExpress/MessagePort/Tcp/private/TcpProtocolConstants.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

template<typename CompletionHandler>
class BasicProtocolReceiverCommand : private AsioExpress::Coroutine
{
public:
  BasicProtocolReceiverCommand(
      AsioExpress::MessagePort::Tcp::SocketPointer socket,
      AsioExpress::MessagePort::DataBufferPointer buffer,
      CompletionHandler completionHandler);

  void operator()(
      boost::system::error_code ec = boost::system::error_code(),
      std::size_t length = 0);

private:

  #pragma pack(push)
  #pragma pack(1)
  struct Header
  {
    char protocolHeader[ProtocolHeaderSize];
    ProtocolVersionType version;
  };
  #pragma pack(pop)

  typedef boost::shared_ptr<Header> HeaderPointer;

  typedef AsioExpress::MessagePort::DataBuffer::SizeType BufferSizeType;
  typedef boost::shared_ptr<BufferSizeType> BufferSizePointer; 

  AsioExpress::MessagePort::Tcp::SocketPointer   m_socket;
  AsioExpress::MessagePort::DataBufferPointer    m_buffer;
  BufferSizePointer                         m_bufferSize;
  HeaderPointer                             m_header;
  CompletionHandler                         m_completionHandler;
};

template<typename CompletionHandler>
BasicProtocolReceiverCommand<CompletionHandler>::BasicProtocolReceiverCommand(
      AsioExpress::MessagePort::Tcp::SocketPointer socket,
      AsioExpress::MessagePort::DataBufferPointer buffer,
      CompletionHandler completionHandler) :
  m_socket(socket),
  m_buffer(buffer),
  m_bufferSize(new BufferSizeType),
  m_header(new Header),
  m_completionHandler(completionHandler)
{
}

template<typename CompletionHandler>
void BasicProtocolReceiverCommand<CompletionHandler>::operator()(
    boost::system::error_code ec, std::size_t)
{
  if (ec)
  {
    m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ec));
    return;
  }

  REENTER(this)
  {
    // Receive the header from the socket.
    YIELD 
    {
      boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_header.get(), sizeof(Header)), 
        *this);
    }

    // Receive the buffer size from the socket.
    YIELD 
    {
      int memCheck = memcmp(
        m_header->protocolHeader, 
        ProtocolHeaderText, 
        ProtocolHeaderSize);
      if (memCheck != 0)
      {
        m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ErrorCode::ProtocolError));
        return;             
      }

      if (m_header->version != ProtocolVersionBasic)
      {
        m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ErrorCode::WrongProtocolVersion));
        return;             
      }

      boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_bufferSize.get(), sizeof(BufferSizeType)), 
        *this);
    }

    // Receive the buffer.
    YIELD 
    {
      m_buffer->Resize(*m_bufferSize);

      boost::asio::async_read(
        *m_socket,
        boost::asio::buffer(m_buffer->Get(), m_buffer->Size()), 
        *this);    
    }

    m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ec));
  }
}

class BasicProtocolReceiver
{
public:
  template<typename CompletionHandler>
  void AsyncRun(
      AsioExpress::MessagePort::Tcp::SocketPointer socket,
      AsioExpress::MessagePort::DataBufferPointer buffer,
      CompletionHandler completionHandler)
  {
    BasicProtocolReceiverCommand<CompletionHandler>(
      socket, buffer, completionHandler)();
  }
};

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
