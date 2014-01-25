//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef _MSC_VER

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#pragma warning(disable:4503)
#pragma warning(disable: 4505) // "unreference local function removed" bug with templates

#define WIN_DISABLE_WARNINGS_BEGIN(args) __pragma(warning(push)) __pragma(warning(disable: args))
#define WIN_DISABLE_WARNINGS_END __pragma(warning(pop))

#else // _MSC_VER

#define WIN_DISABLE_WARNINGS_BEGIN(args)  
#define WIN_DISABLE_WARNINGS_END

#endif // _MSC_VER

#define BOOST_INTERPROCESS_ENABLE_TIMEOUT_WHEN_LOCKING
#define BOOST_INTERPROCESS_TIMEOUT_WHEN_LOCKING_DURATION_MS 5000

// Turn on debugging of IPC message port.
//#define DEBUG_IPC
