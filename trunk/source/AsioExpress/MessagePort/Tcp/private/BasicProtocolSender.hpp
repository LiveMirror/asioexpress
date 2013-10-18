//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/function.hpp>
#include <boost/asio.hpp>

#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/CompletionHandler.hpp"

#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/MessagePort/Tcp/private/SocketPointer.hpp"
#include "AsioExpress/MessagePort/Tcp/private/ProtocolConstants.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

template<typename CompletionHandler>
class BasicProtocolSenderCommand : private AsioExpress::Coroutine
{
public:
  BasicProtocolSenderCommand(
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
    explicit Header(AsioExpress::MessagePort::DataBuffer::SizeType length) :
      version(ProtocolVersionBasic),
      length(length)
    {
      memcpy_s(
        protocolHeader, 
        sizeof(protocolHeader), 
        ProtocolHeaderText, 
        ProtocolHeaderSize);
    }
    char protocolHeader[ProtocolHeaderSize];
    ProtocolVersionType version;
    AsioExpress::MessagePort::DataBuffer::SizeType length;
  };
  #pragma pack(pop)

  AsioExpress::MessagePort::Tcp::SocketPointer   m_socket;
  AsioExpress::MessagePort::DataBufferPointer    m_buffer;
  CompletionHandler                         m_completionHandler;
};

template<typename CompletionHandler>
BasicProtocolSenderCommand<CompletionHandler>::BasicProtocolSenderCommand(
    AsioExpress::MessagePort::Tcp::SocketPointer socket,
    AsioExpress::MessagePort::DataBufferPointer buffer,
    CompletionHandler completionHandler) :
  m_socket(socket),
  m_buffer(buffer),
  m_completionHandler(completionHandler)
{
}

template<typename CompletionHandler>
void BasicProtocolSenderCommand<CompletionHandler>::operator()(
    boost::system::error_code ec, std::size_t)
{
  if (ec)
  {
    m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ec));
    return;
  }

  REENTER(this)
  {
    // Send the buffer size.
    YIELD 
    {
      Header header(m_buffer->Size());

      boost::asio::async_write(
        *m_socket,
        boost::asio::buffer(&header, sizeof Header), 
        *this);
    }

    // Send buffer.
    YIELD 
      boost::asio::async_write(
        *m_socket,
        boost::asio::buffer(m_buffer->Get(), m_buffer->Size()),
        *this);

    m_socket->get_io_service().post(boost::asio::detail::bind_handler(m_completionHandler, ec));
  }
}

class BasicProtocolSender
{
public:
  template<typename CompletionHandler>
  void AsyncRun(
      AsioExpress::MessagePort::Tcp::SocketPointer socket,
      AsioExpress::MessagePort::DataBufferPointer buffer,
      CompletionHandler completionHandler)
  {
    BasicProtocolSenderCommand<CompletionHandler>(
      socket, buffer, completionHandler)();
  }
};

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
