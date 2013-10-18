//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/DebugTimer/DebugTimers.hpp"
#include "AsioExpress/DebugTimer/ScopedDebugTimer.hpp"

#if defined(_DEBUG) // && defined(ENABLE_DEBUG_TIMERS)

///
/// Sets the threshold for which timer durations will be displayed to the debug
/// output stream. Durations that are less than the threshold will not be 
/// printed.
/// @param threshold Threshold time in milliseconds. 
///
#define SET_DEBUG_TIMER_PRINT_THRESHOLD(threshold) AsioExpress::SetDebugTimerPrintThreshold(threshold)

///
/// Starts a timer.
/// @param name The name of the timer.
///
#define START_DEBUG_TIMER(name) AsioExpress::StartDebugTimer(name)

///
/// Stops a timer and prints out the statistics for the timer to the debug
/// output stream.
/// @param name The name of the timer.
///
#define STOP_DEBUG_TIMER(name) AsioExpress::StopDebugTimer(name)

///
/// Starts a timer if it is not already running, otherwise, it prints out the 
/// statistics for the timer to the debug output stream.
/// @param name The name of the timer.
/// @param file The name of the source file.
/// @param line The line number of the source file.
///
#define STATEMENT_DEBUG_TIMER(name, file, line) AsioExpress::StatementDebugTimer(name, file, line); AsioExpress::ScopedDebugTimer myScopedDebugTimer(name, file, line)

///
/// Use this macro to stop the statement timer. Nothing is printed to the output
/// debug stream.
/// @param name The name of the timer.
/// @param file The name of the source file.
/// @param line The line number of the source file.
///
#define STOP_STATEMENT_DEBUG_TIMER(name, file, line) AsioExpress::StopStatementDebugTimer(name, file, line)

///
/// Use this macro to deallocate all of the debug timers. 
///
#define CLEAN_UP_DEBUG_TIMERS AsioExpress::CleanUpDebugTimers()

///
/// Use this macro to display a list of the active debug timers.
///
#define DISPLAY_DEBUG_TIMERS AsioExpress::DisplayDebugTimers()

#define DECLARE_TIMER_ID

#else

#define SET_DEBUG_TIMER_PRINT_THRESHOLD(threshold)    ((void)0)
#define START_DEBUG_TIMER(name)                       ((void)0)
#define STOP_DEBUG_TIMER(name)                        ((void)0)
#define STATEMENT_DEBUG_TIMER(name, file, line)       ((void)0)
#define STOP_STATEMENT_DEBUG_TIMER(name, file, line)  ((void)0)
#define CLEAN_UP_DEBUG_TIMERS                         ((void)0)
#define DISPLAY_DEBUG_TIMERS                          ((void)0)

#endif // _DEBUG
