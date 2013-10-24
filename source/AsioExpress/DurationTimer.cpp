//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <limits>
#include <cassert>

#include "AsioExpress/DurationTimer.hpp"
#include "AsioExpress/Platform/GetClockCount.hpp"

using namespace std;

namespace AsioExpress {

DurationTimer::DurationTimer() :
  startTime(GetClockCount())
{
}

unsigned int DurationTimer::duration()
{
  return getDuration(startTime, GetClockCount());
}

void DurationTimer::reset()
{
  startTime = GetClockCount();
}

} // namespace AsioExpress
