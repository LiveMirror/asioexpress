//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <sstream>

#ifdef _DEBUG

#include "AsioExpress/DebugTimer/DebugTimerManager.hpp"
#include "AsioExpress/DebugTimer/DebugTimers.hpp"

namespace
{
  unsigned long DebugTimerPrintThreshold = 25;
};

namespace AsioExpress {

static std::string GetCategory(std::string const & name)
{
  std::string category(name);

  std::string::size_type pos = category.find_first_of('-');
  if (pos == std::string::npos)
  {
    return std::string();
  }

  return category.substr(0, pos);
}

static void ProcessTimerStats(
    TimerStats stats, 
    std::string const & name, 
    char const * timerType,
    char const * file, 
    int line)
{
  if (stats.duration >= DebugTimerPrintThreshold)
  {
    std::ostringstream message;
    message
      << timerType
      << ":        " 
      << name;
    if (file!=NULL)
      message << "; " << file << ":" << line;
    message
      << "; duration=" << stats.duration
      << "; low=" << stats.lowValue
      << "; high=" << stats.highValue
      << "; totalTime=" << stats.totalTime
      << "; runs=" << stats.runs
      << "\n";
    OutputDebugString(message.str().c_str());
  }

  std::string category = GetCategory(name);

  if (!category.empty())
  {
    TimerStats & runningStats
      = DebugTimerManager::Instance()->GetTimerStats(category);

    if (stats.lowValue < runningStats.lowValue)
      runningStats.lowValue = stats.lowValue;
    if (stats.highValue > runningStats.highValue)
      runningStats.highValue = stats.highValue;
    runningStats.runs += stats.runs;
    runningStats.totalTime += stats.totalTime;

    if (stats.duration >= DebugTimerPrintThreshold)
    {
      std::ostringstream message;
      message 
        << timerType
        << "Summary: " 
        << category
        << "; low=" << runningStats.lowValue
        << "; high=" << runningStats.highValue
        << "; totalTime=" << runningStats.totalTime
        << "; runs=" << runningStats.runs
        << "\n";
      OutputDebugString(message.str().c_str());
    }

    DebugTimerManager::Instance()->RemoveTimer(name);
  }
}

void SetDebugTimerPrintThreshold(int threshold)
{
  DebugTimerPrintThreshold = threshold;
}

void StartDebugTimer(char const * name)
{
  StatisticsTimer & timer = DebugTimerManager::Instance()->GetTimer(name);

  timer.start();
  timer.setDescription(name);
}

void StopDebugTimer(char const * name)
{
  std::string category = GetCategory(name);

  StatisticsTimer & timer = DebugTimerManager::Instance()->GetTimer(name);

  if (!timer.enabled())
  {
    DebugTimerManager::Instance()->RemoveTimer(name);
    return;
  }

  TimerStats stats = timer.duration();
  timer.stop();

  ProcessTimerStats(stats, name, "StatementTimer", NULL, 0);  
}

void StatementDebugTimer(char const * name, char const * file, int line)
{
  StatementDebugTimer(std::string(name), file, line);
}

void StatementDebugTimer(std::string name, char const * file, int line)
{
  std::ostringstream description;
  description << name << "; " << file << ":" << line;
  
  StatisticsTimer & timer = DebugTimerManager::Instance()->GetTimer(name);

  if (! timer.enabled())
  {
    timer.start();
    timer.setDescription(description.str());
    if (DebugTimerPrintThreshold <= 0)
    {
      std::ostringstream message;
      message 
        << "StatementTimerStarted: "
        << description.str()
        << "\n";
      OutputDebugString(message.str().c_str());
    }

    return;
  }

  TimerStats stats = timer.duration();
  timer.reset();
  timer.setDescription(description.str());

  if (stats.duration >= DebugTimerPrintThreshold)
  {
    std::ostringstream message;
    message 
      << "StatementTimer:        "
      << name
      << "; " << file << ":" << line
      << "; duration=" << stats.duration
      << "; low=" << stats.lowValue
      << "; high=" << stats.highValue
      << "; totalTime=" << stats.totalTime
      << "; runs=" << stats.runs
      << "\n";
    OutputDebugString(message.str().c_str());
  }
}

void StopStatementDebugTimer(char const * name, char const * file, int line)
{
  StopStatementDebugTimer(std::string(name), file, line);
} 

void StopStatementDebugTimer(std::string name, char const * file, int line)
{
  std::string category = GetCategory(name);

  StatisticsTimer & timer = DebugTimerManager::Instance()->GetTimer(name);

  if (!timer.enabled())
  {
    DebugTimerManager::Instance()->RemoveTimer(name);
    return;
  }

  TimerStats stats = timer.duration();
  timer.stop();

  ProcessTimerStats(stats, name, "StatementTimer", file, line);  

  if (DebugTimerPrintThreshold <= 0)
  {
    std::ostringstream message;
    message 
      << "StatementTimerStopped: "
      << name
      << "\n";
    OutputDebugString(message.str().c_str());
  }
}

void FunctionExitDebugTimer(std::string const & name, char const * const file, int line)
{
  StatisticsTimer & timer = DebugTimerManager::Instance()->GetTimer(name);

  unsigned long duration = timer.elapsed();

  if (duration >= DebugTimerPrintThreshold)
  {
    std::ostringstream message;
    message
      //                         "
      << "FunctionDebugTimer:    "
      << name
      << "; " << file << ":" << line
      << "; executionTime=" << duration
      << "\n";
    OutputDebugString(message.str().c_str());
  }
}

void CleanUpDebugTimers()
{
  AsioExpress::DebugTimerManager::CleanUp();
}

void DisplayDebugTimers()
{
  DebugTimerManager * timerManager = DebugTimerManager::Instance();

  DebugTimerManager::TimerNames timers;
  timerManager->GetTimerNames(timers);

  OutputDebugString("Active debug timers:\n");
  DebugTimerManager::TimerNames::const_iterator name = timers.begin();
  DebugTimerManager::TimerNames::const_iterator  end = timers.end();
  for(; name != end; ++name)
  {
    StatisticsTimer & timer = timerManager->GetTimer(*name);
    if (timer.enabled())
    {
      OutputDebugString("  ");
      OutputDebugString(timer.getDescription().c_str());
      OutputDebugString("\n");
    }
  }
  OutputDebugString("End of active debug timers.\n");
}

} // namespace AsioExpress

#endif // _DEBUG
