//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <limits>
#include <cassert>

#include "AsioExpress/StatisticsTimer.hpp"

using namespace std;

namespace AsioExpress {

StatisticsTimer::StatisticsTimer() :
  isEnabled(false)
{
}

void StatisticsTimer::start()
{
  isEnabled = true;
  timer.reset();
}

void StatisticsTimer::stop()
{
  isEnabled = false;
}

void StatisticsTimer::reset()
{
  timer.reset();
}

bool StatisticsTimer::enabled() const
{
  return isEnabled;
}

TimerStats StatisticsTimer::duration()
{
  assert(isEnabled);

  stats.duration = timer.duration();

  if(stats.duration < stats.lowValue)
    stats.lowValue = stats.duration;

  if(stats.duration > stats.highValue)
    stats.highValue = stats.duration;

  ++stats.runs;

  stats.totalTime += stats.duration;  

  return stats;
}

unsigned long StatisticsTimer::elapsed()
{
  return timer.duration();
}

void StatisticsTimer::setDescription(std::string newDescription)
{
  description = newDescription;
}

std::string StatisticsTimer::getDescription()
{
  return description;
}

} // namespace AsioExpress
