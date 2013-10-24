//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <limits>

#undef max // This macro is causing compile errors

namespace AsioExpress {

class DurationTimer
{
public:
  DurationTimer();

  unsigned int duration();

  void reset();

private:
  unsigned int startTime;
};

inline unsigned int getDuration(unsigned int time0, unsigned int time1)
{
  return time0 <= time1 ? time1 - time0 : std::numeric_limits<unsigned int>::max() - time0 + time1;
}

} // namespace AsioExpress
