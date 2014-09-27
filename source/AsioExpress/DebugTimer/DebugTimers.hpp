//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#ifdef _DEBUG

namespace AsioExpress {

extern void SetDebugTimerPrintThreshold(int threshold);

extern void StartDebugTimer(char const * name);
extern void StopDebugTimer(char const * name);
extern void RemoveDebugTimer(char const * name);

extern void StatementDebugTimer(std::string name, char const * file, int line);
extern void StatementDebugTimer(char const * name, char const * file, int line);
extern void RemoveStatementDebugTimer(std::string name, char const * file, int line);
extern void RemoveStatementDebugTimer(char const * name, char const * file, int line);
extern void FunctionExitDebugTimer(std::string const & name, char const * const file, int line);

extern void CleanUpDebugTimers();

extern void DisplayDebugTimers();

} // namespace AsioExpress

#endif // _DEBUG
