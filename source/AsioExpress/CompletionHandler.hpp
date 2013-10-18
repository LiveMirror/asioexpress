//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "boost/function.hpp"
#include "boost/asio/io_service.hpp"
#include "AsioExpress/Error.hpp"

namespace AsioExpress {

// using a global variable to increase performance.
extern bool g_isDirectCompletionHanderCalls;

typedef boost::function<void (AsioExpress::Error)> CompletionHandler;

inline void SetDirectCompletionHanderCalls(bool enable=true)
{
  g_isDirectCompletionHanderCalls = enable;
}

inline void CallCompletionHandler(
    boost::asio::io_service & ioService, 
    CompletionHandler completionHandler,
    AsioExpress::Error error)
{
  if (g_isDirectCompletionHanderCalls)
  {
    // For unit testing purposes the compeltion hander is called directly.
    completionHandler(error);
  }
  else
  {
    // We post to the handler to the IO service to avoid stack overflow.
    ioService.post(boost::asio::detail::bind_handler(completionHandler, error));
  }
}

} // namespace AsioExpress
