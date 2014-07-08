//               Copyright Ross MacGregor 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Timer/StandardDateTimer.hpp"

#include "AsioExpress/Timer/DateTimerFactory.hpp"

namespace AsioExpress {

///
/// This class creates a timer factory for creating new DateTimer's.
///
class DateTimerAllocator : public DateTimerFactory
{
public:

  DateTimerAllocator(boost::asio::io_service & ioService) :
    ioService(&ioService)
  {
  }

  ///
  /// Create a new date timer.
  /// @returns Returns a DateTimer pointer.
  ///
  virtual DateTimerPointer NewDateTimer()
  {
      return DateTimerPointer(new StandardDateTimer(*ioService));
  }

private:
  boost::asio::io_service * ioService;
};

} // namespace AsioExpress
