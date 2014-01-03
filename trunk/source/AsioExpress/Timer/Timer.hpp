//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/CompletionHandler.hpp"

namespace AsioExpress {

class Timer
{
public:
  virtual ~Timer() {};

  virtual void SetWaitTime(
      unsigned int milliseconds) = 0;

  virtual void AsyncWait(
      CompletionHandler completionHandler) = 0;

  virtual void AsyncWait(
      unsigned int milliseconds, 
      CompletionHandler completionHandler) = 0;

  virtual void Stop() = 0;
};

typedef boost::shared_ptr<Timer> TimerPointer;

} // namespace AsioExpress
