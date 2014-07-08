//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Timer/StandardTimer.hpp"
#include "AsioExpress/Timer/StandardDateTimer.hpp"
#include "AsioExpress/Timer/StandardRepeatingTimer.hpp"

#include "AsioExpress/Timer/TimerAllocator.hpp"
#include "AsioExpress/Timer/DateTimerAllocator.hpp"

using namespace AsioExpress;
using namespace std;

BOOST_AUTO_TEST_SUITE(TimerAllocatorTest)

BOOST_AUTO_TEST_CASE(TestStandardTimer)
{
  typedef TimerAllocator<StandardTimer> TimerAllocator;

  boost::asio::io_service ioService;
  TimerFactoryPointer timers(new TimerAllocator(ioService));

  TimerPointer timer1 = timers->NewTimer();
  TimerPointer timer2 = timers->NewTimer();

  BOOST_CHECK(timer1);
  BOOST_CHECK(timer2);
  BOOST_CHECK(timer1 != timer2);
}

BOOST_AUTO_TEST_CASE(TestStandardRepeatingTimer)
{
  typedef TimerAllocator<StandardRepeatingTimer> TimerAllocator;

  boost::asio::io_service ioService;
  TimerFactoryPointer timers(new TimerAllocator(ioService));

  TimerPointer timer1 = timers->NewTimer();
  TimerPointer timer2 = timers->NewTimer();

  BOOST_CHECK(timer1);
  BOOST_CHECK(timer2);
  BOOST_CHECK(timer1 != timer2);
}

BOOST_AUTO_TEST_CASE(TestStandardDateTimer)
{
  boost::asio::io_service ioService;
  DateTimerFactoryPointer timers(new DateTimerAllocator(ioService));

  DateTimerPointer timer1 = timers->NewDateTimer();
  DateTimerPointer timer2 = timers->NewDateTimer();

  BOOST_CHECK(timer1);
  BOOST_CHECK(timer2);
  BOOST_CHECK(timer1 != timer2);
}

BOOST_AUTO_TEST_SUITE_END()
