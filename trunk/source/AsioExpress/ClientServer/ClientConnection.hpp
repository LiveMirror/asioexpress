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
    ioService(&ioService),
    messagePortId(id),
    client(client)
  {
  }

  ClientConnection & operator=(ClientConnection const &that)
  {
      if (this != &that)
      {
          this->ioService = that.ioService;
          this->messagePortId = that.messagePortId;
          this->client = that.client;
      }
          
      return *this;
  }

  boost::asio::io_service & GetIoService() const
  {
    return *ioService;
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
  boost::asio::io_service *   ioService;
  MessagePortId               messagePortId;
  ClientInterfacePointer      client;
};

} // namespace MessagePort
} // namespace AsioExpress
