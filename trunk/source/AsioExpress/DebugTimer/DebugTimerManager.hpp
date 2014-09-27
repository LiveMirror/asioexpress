//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef _DEBUG

#include <map>

#include "AsioExpress/StatisticsTimer.hpp"

namespace AsioExpress {

class DebugTimerManager
{
public:
  typedef std::vector<std::string> TimerNames;

  void AddTimer(std::string timerName);

  bool HasTimer(std::string timerName);

  StatisticsTimer & GetTimer(std::string timerName);

  void RemoveTimer(std::string timerName);

  void GetTimerNames(TimerNames &names);

  TimerStats & GetTimerStats(std::string timerName);

  static DebugTimerManager* Instance();

  static void CleanUp();

private:
  typedef std::map<std::string, AsioExpress::StatisticsTimer> TimerMap;
  typedef std::map<std::string, AsioExpress::TimerStats> TimerStatsMap;

  TimerMap                      m_timers;
  TimerStatsMap                 m_timerStats;
  static DebugTimerManager *    m_instance;
};

} // namespace AsioExpress

#endif // _DEBUG
