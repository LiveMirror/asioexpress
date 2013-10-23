//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressConfig/config.hpp"

#include "AsioExpress/Timer/TickCountTraits.hpp"
#include "AsioExpress/Timer/BasicRepeatingTimer.hpp"
#include <stdint.h>

namespace AsioExpress {

typedef BasicRepeatingTimer<
  uint32_t, 
  tick_count_traits, 
  boost::asio::deadline_timer_service<
    uint32_t, 
    tick_count_traits> > RepeatingTickCountTimer;

} // namespace AsioExpress
