//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"
#include "sys/timeb.h"

namespace AsioExpress {

unsigned int GetClockCount(void)
{
  timeb tb;
  ftime( &tb );
  int count = tb.millitm + (tb.time & 0x3fffff) * 1000;
  return count;
}

} // namespace AsioExpress
