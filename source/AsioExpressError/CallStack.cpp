//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/CallStack.hpp"

using namespace AsioExpress;

namespace
{
  CallStackServiceHandler callStackServiceHandler;
}

void AsioExpress::SetCallStackHandler(
    CallStackServiceHandler handler)
{
  CHECK(handler != 0);

  callStackServiceHandler = handler;
}

std::string AsioExpress::GetCallStack()
{
  if (callStackServiceHandler == 0)
  {
    return std::string("The call stack service not available. See function AsioExpress::SetCallStackHandler.");
  }
  else
  {
    return callStackServiceHandler();
  }
}

std::string AsioExpress::AppendCallStack(
    std::string message)
{
  message += "\nCall Stack:\n";
  message += GetCallStack();
  return message;
}
