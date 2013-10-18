//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#include "AsioExpressConfig/config.hpp"
#include <windows.h>

#include <exception>
#include <sstream>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4244 4512 4245)
#include <boost/function.hpp>
#pragma warning(pop)
