//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/MessagePortId.hpp"
#include "AsioExpress/ClientServer/ServerInterface.hpp"

namespace AsioExpress {
namespace MessagePort {

struct ServerConnection
{
  ServerConnection(
      boost::asio::io_service & ioService,
      MessagePortId id, 
      ServerInterfacePointer server) :
    ioService(ioService),
    messagePortId(id),
    server(server)
  {
  }

  boost::asio::io_service & GetIoService() const
  {
    return ioService;
  }

  MessagePortId GetMessagePortId() const
  {
    return messagePortId;
  }
  
  ServerInterfacePointer GetServer() const
  {
    return server;
  }

private:
  ServerConnection & operator=(ServerConnection const &);

  boost::asio::io_service &   ioService;
  MessagePortId               messagePortId;
  ServerInterfacePointer      server;
};

} // namespace MessagePort
} // namespace AsioExpress
