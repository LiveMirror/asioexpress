//               Copyright Ross MacGregor 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Timer/TimerFactory.hpp"

namespace AsioExpress {

///
/// This template class creates a timer factory for a specific timer type.
/// The timer type must be based upon the Timer interface.
/// Works with StandardTimer and StandardRepeatingTimer types as template
/// parameters.
///
template <class T>
class TimerAllocator : public TimerFactory
{
public:
  ///
  /// The constructor requires an io_service reference for creating new timers.
  /// @param ioService -- The io_service used to run the new timers.
  ///
  TimerAllocator(boost::asio::io_service & ioService) :
    ioService(&ioService)
  {
  }

  virtual TimerPointer NewTimer()
  {
      return TimerPointer(new T(*ioService));
  }

private:
  boost::asio::io_service * ioService;
};

} // namespace AsioExpress
