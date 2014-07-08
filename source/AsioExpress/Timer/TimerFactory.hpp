//               Copyright Ross MacGregor 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/Timer/Timer.hpp"

namespace AsioExpress {

class TimerFactory
{
public:
  virtual ~TimerFactory() {};

  virtual TimerPointer NewTimer() = 0;
};

typedef boost::shared_ptr<TimerFactory> TimerFactoryPointer;

} // namespace AsioExpress
