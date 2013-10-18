//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/Windows/WinCallStack.hpp"

using namespace AsioExpress;

namespace
{
  WinCallStackServiceHandler callStackServiceHandler;
}

void AsioExpress::SetWinCallStackHandler(
    WinCallStackServiceHandler handler)
{
  CHECK(handler != 0);

  callStackServiceHandler = handler;
}

std::string AsioExpress::GetWinCallStack(
    HANDLE thread, 
    const CONTEXT *context)
{
  if (callStackServiceHandler == 0)
  {
    return std::string("The call stack service not available. See function AsioExpress::SetCallStackHandler.");
  }
  else
  {
    return callStackServiceHandler(thread, context);
  }
}
