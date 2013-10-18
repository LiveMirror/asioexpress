//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#endif
#include "AsioExpressConfig/config.hpp"
#include <windows.h>

#include "AsioExpress/Timer/TickCountTraits.hpp"
#include "AsioExpress/Timer/BasicRepeatingTimer.hpp"

namespace AsioExpress {

typedef BasicRepeatingTimer<
  DWORD, 
  tick_count_traits, 
  boost::asio::deadline_timer_service<
    DWORD, 
    tick_count_traits> > RepeatingTickCountTimer;

} // namespace AsioExpress
