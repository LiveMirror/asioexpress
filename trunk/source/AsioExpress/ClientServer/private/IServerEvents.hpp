//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/ClientServer/ServerConnection.hpp"
#include "AsioExpress/ClientServer/ServerMessage.hpp"

namespace AsioExpress {
namespace MessagePort {

class IServerEvents
{
public:
  virtual ~IServerEvents() {}

  virtual AsioExpress::Error HandleConnected(
    ServerConnection connection) = 0;

  virtual void HandleDisconnected(
    ServerConnection connection,
    AsioExpress::Error error) = 0;

  virtual void HandleMessage(
    ServerMessage message) = 0;

  virtual AsioExpress::Error HandleMessageError(
    ServerMessage message,
    AsioExpress::Error error) = 0;
};

typedef boost::shared_ptr<IServerEvents> IServerEventsPointer;

} // namespace MessagePort
} // namespace AsioExpress
