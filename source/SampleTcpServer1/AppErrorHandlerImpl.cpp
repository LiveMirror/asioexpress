//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include "AsioExpress/Logging/Macros.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "SampleTcpServer1/Logger.hpp"
#include "SampleTcpServer1/Terminate.hpp"
#include "SampleTcpServer1/AppErrorHandlerImpl.hpp"

static Logger logger("SampleServer1", 5);

AsioExpress::Error CommonErrorHandler(
    AsioExpress::MessagePort::ServerConnection & connection,
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
    
    // TODO: we may need API to disconnect the client in the future. 
    // server.Diconnect(messagePortId);

    //this error has to return to the event handler to ensure the message port is disconnected
    return error;
  }

  Terminate(error);

  return error;
}

AsioExpress::Error AppErrorHandlerImpl::ConnectionError(
    AsioExpress::MessagePort::ServerConnection connection, 
    AsioExpress::Error error)
{
  return CommonErrorHandler(connection, error);
}

AsioExpress::Error AppErrorHandlerImpl::MessageError(
    AsioExpress::MessagePort::ServerMessage message, 
    AsioExpress::Error error)
{
  return CommonErrorHandler(message.GetConnection(), error);
}
