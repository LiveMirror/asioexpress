//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/ClientServer/ClientConnection.hpp"
#include "AsioExpress/ClientServer/ClientMessage.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientEvents
{
public:
  virtual ~ClientEvents() {}

  virtual AsioExpress::Error HandleConnected(
    ClientConnection connection) = 0;

  virtual void HandleDisconnected(
    ClientConnection connection, 
    AsioExpress::Error error) = 0;

  virtual void HandleMessage(
    ClientMessage message) = 0;

  virtual AsioExpress::Error HandleMessageError(
    ClientMessage message,
    AsioExpress::Error error) = 0;
};

typedef boost::shared_ptr<ClientEvents> ClientEventsPointer;

} // namespace MessagePort
} // namespace AsioExpress
