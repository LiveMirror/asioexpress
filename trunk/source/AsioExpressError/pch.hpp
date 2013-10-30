//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressConfig/config.hpp"

#ifdef _MSC_VER
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
    #include <windows.h>
#endif // _MSC_VER

#include <exception>
#include <sstream>
#include <string>

WIN_DISABLE_WARNINGS_BEGIN(4244 4512 4245)
#include <boost/function.hpp>
WIN_DISABLE_WARNINGS_END

#include <exception>
#include <sstream>
#include <string>
#include <boost/function.hpp>
