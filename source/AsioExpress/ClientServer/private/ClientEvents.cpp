//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <boost/bind.hpp>

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/ClientServer/private/IClientEvents.hpp"
#include "AsioExpress/ClientServer/private/ClientEvents.hpp"

namespace AsioExpress {
namespace MessagePort {

ClientEvents::ClientEvents(ClientEventHandler * eventHandler) :
  m_eventHandler(eventHandler)
{
}

AsioExpress::Error ClientEvents::HandleConnected(
  ClientConnection connection)
{
  try
  {
    m_eventHandler->ClientConnected(connection);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(return error)

  return AsioExpress::Error();
}

void ClientEvents::HandleDisconnected(
  ClientConnection connection, 
  AsioExpress::Error error)
{
  try
  {
    m_eventHandler->ClientDisconnected(connection, error);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(m_eventHandler->ConnectionError(connection, error))
}

void ClientEvents::HandleMessage(
  ClientMessage message)
{
  try
  {
    m_eventHandler->AsyncProcessMessage(message);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(message.CallCompletionHandler(error))
}

AsioExpress::Error ClientEvents::HandleMessageError(
  ClientMessage message,
  AsioExpress::Error error)
{
  try
  {
    return m_eventHandler->MessageError(message, error);
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO(return error)
}

} // namespace MessagePort
} // namespace AsioExpress
