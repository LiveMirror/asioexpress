//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#ifdef _DEBUG

#include "AsioExpress/DebugTimer/DebugTimerManager.hpp"

namespace AsioExpress {

DebugTimerManager * DebugTimerManager::m_instance;

void DebugTimerManager::AddTimer(std::string timerName)
{
  m_timers[timerName] = StatisticsTimer();
}

bool DebugTimerManager::HasTimer(std::string timerName)
{
  return m_timers.count(timerName) == 1;
}

StatisticsTimer & DebugTimerManager::GetTimer(std::string timerName)
{
  return m_timers[timerName];
}

void DebugTimerManager::RemoveTimer(std::string timerName)
{
  m_timers.erase(timerName);
}

void DebugTimerManager::GetTimerNames(TimerNames &names)
{
  TimerMap::const_iterator  it = m_timers.begin();
  TimerMap::const_iterator end = m_timers.end();
  for(; it != end; ++it)
  {
    names.push_back(it->first);
  }
}

TimerStats & DebugTimerManager::GetTimerStats(std::string timerName)
{
  return m_timerStats[timerName];
}

DebugTimerManager* DebugTimerManager::Instance()
{
  if (m_instance == NULL)
    m_instance = new DebugTimerManager;

  return m_instance;
}

void DebugTimerManager::CleanUp()
{
  delete m_instance;
  m_instance = NULL;
}

} // namespace AsioExpress

#endif // _DEBUG
