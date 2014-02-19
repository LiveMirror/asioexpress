//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "boost/function.hpp"
#include "boost/asio/io_service.hpp"
#include "AsioExpress/Error.hpp"

namespace AsioExpress {

// using a global variable to increase performance.
extern bool g_isUnitTestMode;

inline void SetUnitTestMode(bool enable=true)
{
  g_isUnitTestMode = enable;
}

} // namespace AsioExpress
