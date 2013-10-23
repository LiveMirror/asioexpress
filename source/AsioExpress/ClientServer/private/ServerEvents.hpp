//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/ClientServer/private/IServerEvents.hpp"
#include "AsioExpress/ClientServer/private/MessagePortManager.hpp"
#include "AsioExpress/ClientServer/ServerEventHandler.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerEvents : public IServerEvents
{
public:
  ServerEvents(
      ServerEventHandler * eventHandler);

  virtual ~ServerEvents() {}

  virtual AsioExpress::Error HandleConnected(
    ServerConnection connection);

  virtual void HandleDisconnected(
    ServerConnection connection,
    AsioExpress::Error error);

  virtual void HandleMessage(
    ServerMessage message);

  virtual AsioExpress::Error HandleMessageError(
    ServerMessage message,
    AsioExpress::Error error);

private:
  ServerEventHandlerPointer   m_eventHandler;
};

} // namespace MessagePort
} // namespace AsioExpress
