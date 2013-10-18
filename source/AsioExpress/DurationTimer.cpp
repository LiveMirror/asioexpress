//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <limits>
#include <cassert>

#include "AsioExpress/DurationTimer.hpp"

using namespace std;

namespace AsioExpress {

DurationTimer::DurationTimer() :
  startTime(GetTickCount())
{
}

unsigned long DurationTimer::duration()
{
  return getDuration(startTime, GetTickCount());
}

void DurationTimer::reset()
{
  startTime = GetTickCount();
}

} // namespace AsioExpress
