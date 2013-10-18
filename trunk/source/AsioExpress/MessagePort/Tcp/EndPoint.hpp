//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include <string>

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

class EndPoint
{
public:
  EndPoint(
      std::string address, 
      std::string port) :
    m_address(address),
    m_port(port)
  {
  }

  boost::asio::ip::tcp::endpoint const GetEndPoint(
      boost::asio::io_service & ioService)
  {
    boost::asio::ip::tcp::resolver resolver(ioService);
    boost::asio::ip::tcp::resolver::query query(
      boost::asio::ip::tcp::v4(), m_address, m_port);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    return *iterator;
  }

private:
  EndPoint & operator=(EndPoint const &);

  std::string const m_address;
  std::string const m_port;
};

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
