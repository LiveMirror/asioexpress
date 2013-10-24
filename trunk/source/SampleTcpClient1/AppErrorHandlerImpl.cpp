//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpClient1/pch.hpp"

#include "AsioExpress/Logging/Macros.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "SampleTcpClient1/Logger.hpp"
#include "SampleTcpClient1/Terminate.hpp"
#include "SampleTcpClient1/AppErrorHandlerImpl.hpp"

static Logger logger("SampleClient1", 5);

AsioExpress::Error CommonErrorHandler(
    AsioExpress::MessagePort::ClientConnection connection,
    AsioExpress::Error const & error)
{
  if (error.GetErrorCode() == AsioExpress::ErrorCode::UniqueEventTimeout)
  {
    // Log error.
    LOG_ERROR_STR(connection.GetIoService(), logger, error.Message());

    // No error returned back to the server.
    return AsioExpress::Error();
  }

  if (error.IsAsioError())
  {
    LOG_ERROR(connection.GetIoService(), logger, error.Message());

    // Disconnect the client manually, in case this handler is not called from
    // the event handler.
    connection.GetClient()->Disconnect();

    // This error has to return to the event handler to ensure the message port
    // is disconnected.
    return error;
  }

  Terminate(error);

  return error;
}

AsioExpress::Error AppErrorHandlerImpl::ConnectionError(
    AsioExpress::MessagePort::ClientConnection connection, 
    AsioExpress::Error error)
{
  return CommonErrorHandler(connection, error);
}

AsioExpress::Error AppErrorHandlerImpl::MessageError(
    AsioExpress::MessagePort::ClientMessage message, 
    AsioExpress::Error error)
{
  return CommonErrorHandler(message.GetConnection(), error);
}
