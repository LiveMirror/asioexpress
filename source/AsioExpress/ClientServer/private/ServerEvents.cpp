//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/pch.hpp"

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/ClientServer/private/ServerEvents.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace ClientServer {

ServerEvents::ServerEvents(
    ServerEventHandler * eventHandler) :
  m_eventHandler(eventHandler)
{
}

AsioExpress::Error ServerEvents::HandleConnected(
  ServerConnection connection)
{
  try
  {
    m_eventHandler->ClientConnected(connection);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(return error)

  return AsioExpress::Error();
}

void ServerEvents::HandleDisconnected(
  ServerConnection connection,
  AsioExpress::Error error)
{
  try
  {
    m_eventHandler->ClientDisconnected(connection, error);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(m_eventHandler->ConnectionError(connection, error))
}

void ServerEvents::HandleMessage(
  ServerMessage message)
{
  try
  {
    m_eventHandler->AsyncProcessMessage(message);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(message.CallCompletionHandler(error))
}

AsioExpress::Error ServerEvents::HandleMessageError(
  ServerMessage message,
  AsioExpress::Error error)
{
  try
  {
    return m_eventHandler->MessageError(message, error);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(return error)
}

} // namespace ClientServer
} // namespace MessagePort
} // namespace AsioExpress
