//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/Windows/WinCallStack.hpp"

#ifdef ASIOEXPRESSWINSTACKDLL_EXPORTS
  #define ASIOEXPRESSWINSTACK_API __declspec(dllexport)
#else
  #define ASIOEXPRESSWINSTACK_API __declspec(dllimport)
#endif

namespace AsioExpress {

///
/// Call this function to allocate the stack tracing information that is
/// needed for subsequent GetCallStack function calls. Use this function 
/// to avoid performance penalties the first time you call GetCallStack.
/// 
extern ASIOEXPRESSWINSTACK_API void StartCallStackService();

///
/// Call this function to deallocate the stack tracing information that is
/// allocated on the first call to GetCallStack.
///
extern ASIOEXPRESSWINSTACK_API void StopCallStackService();

///
/// Call this function to obtain the current call stack information.
/// @param thread Get call stack on this thread.
/// @param context Get call stack information for the provided context object.
/// @return Returns a the call stack in the form of a text string.
///
extern ASIOEXPRESSWINSTACK_API std::string WinCallStackHandler(
    HANDLE thread = GetCurrentThread(), 
    CONTEXT const * context = NULL);

///
/// Call this function to obtain the current call stack information.
/// @return Returns a the call stack in the form of a text string.
///
extern ASIOEXPRESSWINSTACK_API std::string CallStackHandler();

inline void InitializeCallStackService()
{
  SetCallStackHandler(&CallStackHandler);
  SetWinCallStackHandler(&WinCallStackHandler);
}

} // AsioExpress 
