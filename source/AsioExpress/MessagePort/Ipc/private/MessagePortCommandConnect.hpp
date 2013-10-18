//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/MessagePort/Ipc/MessagePort.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {


class MessagePortCommandConnect : private AsioExpress::Coroutine
{
public:
  static int const LowConnectionId = 1;
  static int const HighConnectionId = 99;

  inline MessagePortCommandConnect(const EndPoint& endPoint, MessagePort& messagePort, AsioExpress::CompletionHandler completionHandler)
    : m_endPoint(endPoint),
      m_messagePort(messagePort),
      m_completionHandler(completionHandler),
      m_dataBuffer(new DataBuffer()),
      m_work(messagePort.m_ioService)
  { }

  void operator() (AsioExpress::Error e = AsioExpress::Error());

private:
  inline static std::string IntToString(int n)   { std::stringstream ss; std::string str; ss << n; ss >> str; return str; }

private:
  MessagePortCommandConnect & operator=(MessagePortCommandConnect const &);

  EndPoint                                  m_endPoint;                       
  MessagePort&                              m_messagePort;
  AsioExpress::CompletionHandler           m_completionHandler;
  AsioExpress::MessagePort::DataBufferPointer    m_dataBuffer;
  boost::asio::io_service::work             m_work;
};


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
