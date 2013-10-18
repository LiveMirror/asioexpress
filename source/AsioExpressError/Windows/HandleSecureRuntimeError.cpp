//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/Windows/HandleSecureRuntimeError.hpp"
#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/Windows/SecureRunTimeException.hpp"

using namespace std;
using namespace AsioExpress;

namespace
{
  LogRuntimeErrorHandler logErrorHanlder = NULL;
}

static void myInvalidParameterHandler(const wchar_t* /*expression*/,
   const wchar_t* /*function*/, 
   const wchar_t* /*file*/, 
   unsigned int /*line*/, 
   uintptr_t /*pReserved*/)
{
  std::ostringstream oss;
  oss << "myInvalidParameterHandler(): Secure Runtime Error is detected.";

  logErrorHanlder(oss.str());
  throw SecureRunTimeException(oss.str());
}

void AsioExpress::HandleSecureRuntimeError(LogRuntimeErrorHandler handler)
{
  _invalid_parameter_handler oldHandler, newHandler;
   newHandler = myInvalidParameterHandler;
   oldHandler = _set_invalid_parameter_handler(newHandler);
   CHECK(oldHandler == NULL);

   logErrorHanlder = handler;

   // Disable the message box for assertions.
   _CrtSetReportMode(_CRT_ASSERT, 0);
}
