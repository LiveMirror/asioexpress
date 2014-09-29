//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/ClientServer/private/ClientEvents.hpp"
#include "AsioExpress/ClientServer/ClientEventHandler.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientEventsImpl : public ClientEvents
{
public:
  ClientEventsImpl(ClientEventHandler * eventHandler);

  virtual ~ClientEventsImpl() {}

  virtual AsioExpress::Error HandleConnected(
    ClientConnection connection);

  virtual void HandleDisconnected(
    ClientConnection connection, 
    AsioExpress::Error error);

  virtual void HandleMessage(
    ClientMessage message);

  virtual AsioExpress::Error HandleMessageError(
    ClientMessage message,
    AsioExpress::Error error);

private:
  ClientEventHandlerPointer         m_eventHandler;
};

} // namespace MessagePort
} // namespace AsioExpress
