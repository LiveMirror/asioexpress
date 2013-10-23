//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Timer/TickCountTraits.hpp"

namespace AsioExpress {

typedef boost::asio::basic_deadline_timer<
    uint32_t, tick_count_traits> TickCountTimer;

} // namespace AsioExpress
