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

typedef std::string (*WinCallStackServiceHandler)(HANDLE, CONTEXT const *);

///
/// Call this function to bind a call stack handler for use by the error
/// handling system.
///
extern void SetWinCallStackHandler(
    WinCallStackServiceHandler handler);

///
/// Call this function to obtain the current call stack information.
/// @param thread Get call stack on this thread.
/// @param context Get call stack information for the provided context object.
/// @return Returns a the call stack in the form of a text string.
///
extern std::string GetWinCallStack(
    HANDLE thread = 0, 
    CONTEXT const * context = NULL);

} // namespace AsioExpress
