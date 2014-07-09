//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "AsioExpress/Timer/TimerFactory.hpp"
#include "AsioExpressError/Check.hpp"

namespace AsioExpress {
namespace Testing {

class TimerFactoryMock : public AsioExpress::TimerFactory
{
public:
  TimerFactoryMock() :
    index(0),
    calls(0)
  {
  }

  ///
  /// Returns the next timer in the timer queue.
  ///
  virtual TimerPointer NewTimer()
  {
      CHECK(index < timers.size());
      TimerPointer timer = timers[index];
      ++index;
      ++calls;
      return timer;
  }

  ///
  /// Add a timer to the timer queue.
  ///
  void AddTimer(TimerPointer const & timer)
  {
      timers.push_back(timer);
  }

  ///
  /// Resets the call information and the next NewTimer call will return the
  /// first timer from the timer queue.
  ///
  void Reset()
  {
      index = 0;
      calls = 0;
  }

  ///
  /// Clears everything including the timer queue.
  ///
  void Clear()
  {
      Reset();
      timers.clear();
  }

  ///
  /// Returns the number of calls to the NewTimer function.
  ///
  int Calls() const
  {
      return calls;
  }

  ///
  /// Test that all timers have been used.
  ///
  bool AllTimersUsed()
  {
      return timers.size() != 0 && index == timers.size();
  }

private:
  typedef std::vector<TimerPointer> TimerQueue;
  TimerQueue                timers;
  TimerQueue::size_type     index;
  int                       calls;
};

typedef boost::shared_ptr<TimerFactoryMock> TimerFactoryMockPointer;

} // namespace Test
} // namespace AsioExpress
