//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "SampleTcpClient1/AppErrorHandler.hpp"

class AppErrorHandlerImpl : public AppErrorHandler
{
public:
  virtual AsioExpress::Error ConnectionError(
      AsioExpress::MessagePort::ClientConnection, 
      AsioExpress::Error error);

  virtual AsioExpress::Error MessageError(
      AsioExpress::MessagePort::ClientMessage, 
      AsioExpress::Error error);
};
