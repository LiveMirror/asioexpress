//               Copyright Ross MacGregor 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/Timer/DateTimer.hpp"

namespace AsioExpress {

///
/// This is a factory for creating new timers based on the DateTimer interface.
/// See DateTimerAllocator for an implementation.
///
class DateTimerFactory
{
public:
  virtual ~DateTimerFactory() {};

  ///
  /// Create a new date timer.
  /// @returns Returns a DateTimer pointer.
  ///
  virtual DateTimerPointer NewDateTimer() = 0;
};

typedef boost::shared_ptr<DateTimerFactory> DateTimerFactoryPointer;

} // namespace AsioExpress
