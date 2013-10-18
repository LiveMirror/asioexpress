//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/ClientServer/ClientMessage.hpp"
#include "AsioExpress/ClientServer/ClientConnection.hpp"
#include "AsioExpress/Error.hpp"

///
/// The top level error handler for the application.
///
class AppErrorHandler
{
public:
  virtual ~AppErrorHandler() {};

  ///
  /// Handles connection errors for the client.
  ///
  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ClientConnection, 
      AsioExpress::Error error) = 0;

  ///
  /// Handles message errors for the client.
  ///
  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ClientMessage, 
      AsioExpress::Error error) = 0;
};

typedef boost::shared_ptr<AppErrorHandler> AppErrorHandlerPointer;
