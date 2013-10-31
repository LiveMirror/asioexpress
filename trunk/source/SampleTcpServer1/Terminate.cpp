//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include "AsioExpress/Logging/Macros.hpp"

#include "SampleTcpServer1/Logger.hpp"
#include "SampleTcpServer1/Terminate.hpp"

static Logger logger("SampleServer1", 5);

void Terminate(AsioExpress::Error error)
{
  try
  {
    FATAL_STR_DIRECT(logger, error.Message());
  }
  catch(...)
  {
    // do nothing
  }

  // kills application
  abort();
}
