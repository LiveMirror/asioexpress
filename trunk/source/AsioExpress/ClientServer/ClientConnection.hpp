//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/ClientServer/MessagePortId.hpp"
#include "AsioExpress/ClientServer/ClientInterface.hpp"

namespace AsioExpress {
namespace MessagePort {

struct ClientConnection
{
  ClientConnection(
      boost::asio::io_service & ioService,
      MessagePortId id, 
      ClientInterfacePointer client) :
    ioService(ioService),
    messagePortId(id),
    client(client)
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
  
  ClientInterfacePointer GetClient() const
  {
    return client;
  }

private:
  ClientConnection & operator=(ClientConnection const &);

  boost::asio::io_service &   ioService;
  MessagePortId               messagePortId;
  ClientInterfacePointer   client;
};

} // namespace MessagePort
} // namespace AsioExpress
