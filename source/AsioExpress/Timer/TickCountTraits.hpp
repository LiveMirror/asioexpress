//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once 

#if defined(WIN32)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
  #endif
  #include "AsioExpressConfig/config.hpp"
  #include <windows.h>
#else
# error This is for Windows only!
#endif

#ifdef max // the Windows max macro conflicts with the limits library
#undef max
#endif

#include <boost/asio.hpp>
#include <ctime>
#include <cassert>
#include <limits>

namespace AsioExpress {

struct tick_count_traits
{
  // The time type. This type has no constructor that takes a DWORD to ensure
  // that the timer can only be used with relative times.
  class time_type
  {
  public:
    time_type() : ticks_(0) {}
    DWORD get_ticks() const {return ticks_;}
  private:
    friend struct tick_count_traits;
    DWORD ticks_;
  };

  // The duration type.
  class duration_type
  {
  public:
    duration_type() : ticks_(0) {}
    duration_type(long ticks) : ticks_(ticks) {}
    long get_ticks() const {return ticks_;}
  private:
    friend struct tick_count_traits;
    long ticks_;
  };

  // Get the current time.
  static time_type now()
  {
    time_type result;
    result.ticks_ = ::GetTickCount();
    return result;
  }

  // Add a duration to a time.
  static time_type add(const time_type& t, const duration_type& d)
  {
    time_type result;
    result.ticks_ = t.ticks_ + d.ticks_;
    return result;
  }

  // Subtract one time from another.
  static duration_type subtract(const time_type& t1, const time_type& t2)
  {
    // DWORD tick count values wrap periodically, so we'll use a heuristic that
    // says that if subtracting t1 from t2 yields a value smaller than 2^31,
    // then t1 is probably less than t2. This means that we can't handle
    // durations larger than 2^31, which shouldn't be a problem in practice.
    static DWORD const diff_limit(static_cast<DWORD>(1 << 31));
    long duration(0);
    if (t1.ticks_ < t2.ticks_)
    {
      DWORD diff = t2.ticks_ - t1.ticks_;
      if (diff >= diff_limit)
      {
        duration = static_cast<long>(
          std::numeric_limits<DWORD>::max() - t2.ticks_ + t1.ticks_ + 1);
      }
      else
      {
        duration = - static_cast<long>(diff);
      }
    }
    else
    {
      DWORD diff = t1.ticks_ - t2.ticks_;
      if (diff >= diff_limit)
      {
        duration = - static_cast<long>(
          std::numeric_limits<DWORD>::max() - t1.ticks_ + t2.ticks_ + 1);
      }
      else
      {
        duration = static_cast<long>(diff);
      }
    }
    return duration_type(duration);
  }

  // Test whether one time is less than another.
  static bool less_than(const time_type& t1, const time_type& t2)
  {
    bool is_less = subtract(t2, t1).ticks_ > 0;
    return is_less;
  }

  // Convert to POSIX duration type.
  static boost::posix_time::time_duration to_posix_duration(
      const duration_type& d)
  {
    return boost::posix_time::milliseconds(d.ticks_);
  }
};

} // namespace AsioExpress
