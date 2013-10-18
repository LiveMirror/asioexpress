//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ClientServer/ServerMessage.hpp"
#include "AsioExpress/ClientServer/ServerConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerEventHandler
{
public:
  virtual ~ServerEventHandler() {};

  virtual void ClientConnected(
      AsioExpress::MessagePort::ServerConnection) = 0;

  virtual void ClientDisconnected(
      AsioExpress::MessagePort::ServerConnection connection,
      AsioExpress::Error error) = 0;

  virtual void AsyncProcessMessage(
      AsioExpress::MessagePort::ServerMessage message) = 0;
  
  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ServerConnection connection,
      AsioExpress::Error error) = 0;

  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ServerMessage message, 
      AsioExpress::Error error) = 0;
};

typedef boost::shared_ptr<ServerEventHandler> ServerEventHandlerPointer;

} // namespace MessagePort
} // namespace AsioExpress
