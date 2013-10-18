//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressConfig/config.hpp"
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <string>

namespace AsioExpress {

typedef std::string (*CallStackServiceHandler)();

///
/// Call this function to bind a call stack handler for use by the error
/// handling system.
///
extern void SetCallStackHandler(
    CallStackServiceHandler handler);

///
/// Call this function to obtain the current call stack information.
/// @return Returns a the call stack in the form of a text string.
///
extern std::string GetCallStack();

///
/// This function will add a callstack listing to an existing error message.
/// Use this when a new constant string needs to be initialized.
/// @param message The error message to append tho callstack information.
/// @return Returns a new string with the callstack information appended.
///
extern std::string AppendCallStack(std::string message);

} // namespace AsioExpress
