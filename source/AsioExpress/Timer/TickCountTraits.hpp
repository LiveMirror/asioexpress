//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once 

#ifdef max // the Windows max macro conflicts with the limits library
#undef max
#endif

#include <boost/asio.hpp>
#include <ctime>
#include <cassert>
#include <limits>
#include <stdint.h>

#include "AsioExpress/Platform/GetClockCount.hpp"

namespace AsioExpress {

struct tick_count_traits
{
  // The time type. This type has no constructor that takes a uint32_t to ensure
  // that the timer can only be used with relative times.
  class time_type
  {
  public:
    time_type() : ticks_(0) {}
    uint32_t get_ticks() const {return ticks_;}
  private:
    friend struct tick_count_traits;
    uint32_t ticks_;
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
    result.ticks_ = GetClockCount();
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
    // uint32_t tick count values wrap periodically, so we'll use a heuristic that
    // says that if subtracting t1 from t2 yields a value smaller than 2^31,
    // then t1 is probably less than t2. This means that we can't handle
    // durations larger than 2^31, which shouldn't be a problem in practice.
    static uint32_t const diff_limit(static_cast<uint32_t>(1 << 31));
    long duration(0);
    if (t1.ticks_ < t2.ticks_)
    {
      uint32_t diff = t2.ticks_ - t1.ticks_;
      if (diff >= diff_limit)
      {
        duration = static_cast<long>(
          std::numeric_limits<uint32_t>::max() - t2.ticks_ + t1.ticks_ + 1);
      }
      else
      {
        duration = - static_cast<long>(diff);
      }
    }
    else
    {
      uint32_t diff = t1.ticks_ - t2.ticks_;
      if (diff >= diff_limit)
      {
        duration = - static_cast<long>(
          std::numeric_limits<uint32_t>::max() - t1.ticks_ + t2.ticks_ + 1);
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
