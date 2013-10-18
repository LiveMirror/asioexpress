//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Error.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ClientServer/ClientMessage.hpp"
#include "AsioExpress/ClientServer/ClientConnection.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientEventHandler
{
public:
  virtual ~ClientEventHandler() {};

  virtual void ClientConnected(
      AsioExpress::MessagePort::ClientConnection) = 0;

  virtual void ClientDisconnected(
      AsioExpress::MessagePort::ClientConnection connection,
      AsioExpress::Error error) = 0;

  virtual void AsyncProcessMessage(
      AsioExpress::MessagePort::ClientMessage message) = 0;

  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ClientConnection connection, 
      AsioExpress::Error error) = 0;

  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ClientMessage message, 
      AsioExpress::Error error) = 0;
};

typedef boost::shared_ptr<ClientEventHandler> ClientEventHandlerPointer;

} // namespace MessagePort
} // namespace AsioExpress
