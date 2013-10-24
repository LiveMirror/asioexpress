//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <limits>

#include "AsioExpress/DurationTimer.hpp"
#include "AsioExpress/StatisticsTimer.hpp"

namespace AsioExpress {

struct TimerStats
{
  TimerStats() :
    duration(0),
    totalTime(0),
    runs(0),
    highValue(0),
    lowValue(std::numeric_limits<unsigned int>::max())
  {
  }
  unsigned int   duration;
  long long       totalTime;
  unsigned int   runs;
  unsigned int   highValue;
  unsigned int   lowValue;
};

class StatisticsTimer
{
public:
  StatisticsTimer();

  void start();

  void stop();

  void reset();

  bool enabled() const;

  TimerStats duration();

  unsigned int elapsed();

  void setDescription(std::string description);

  std::string getDescription();

private:
  DurationTimer   timer;
  bool            isEnabled;
  TimerStats      stats;
  std::string     description;
};

} // namespace AsioExpress
