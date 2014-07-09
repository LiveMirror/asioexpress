//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/Timer/StandardTimer.hpp"

#include "AsioExpress/Testing/TimerFactoryMock.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(TimerFactoryMockTest)

BOOST_AUTO_TEST_CASE(TestNoTimers)
{
    TimerFactoryMock timerFactory;

    BOOST_CHECK_EQUAL(timerFactory.Calls(), 0);
    BOOST_CHECK_EQUAL(timerFactory.AllTimersUsed(), false);
}

BOOST_AUTO_TEST_CASE(TestAddTimer)
{
    boost::asio::io_service ioService;
    TimerFactoryMock timerFactory;

    TimerPointer timer1 = TimerPointer(new StandardTimer(ioService));

    timerFactory.AddTimer(timer1);

    BOOST_CHECK_EQUAL(timerFactory.Calls(), 0);
    BOOST_CHECK_EQUAL(timerFactory.AllTimersUsed(), false);
}

BOOST_AUTO_TEST_CASE(TestNewTimer1)
{
    boost::asio::io_service ioService;
    TimerFactoryMock timerFactory;

    TimerPointer timer1 = TimerPointer(new StandardTimer(ioService));

    timerFactory.AddTimer(timer1);

    TimerPointer t1 = timerFactory.NewTimer();

    BOOST_CHECK_EQUAL(t1, timer1);
    BOOST_CHECK_EQUAL(timerFactory.Calls(), 1);
    BOOST_CHECK_EQUAL(timerFactory.AllTimersUsed(), true);
}

BOOST_AUTO_TEST_CASE(TestNewTimer2)
{
    boost::asio::io_service ioService;
    TimerFactoryMock timerFactory;

    TimerPointer timer1 = TimerPointer(new StandardTimer(ioService));
    TimerPointer timer2 = TimerPointer(new StandardTimer(ioService));

    timerFactory.AddTimer(timer1);
    timerFactory.AddTimer(timer2);

    TimerPointer t1 = timerFactory.NewTimer();
    TimerPointer t2 = timerFactory.NewTimer();

    BOOST_CHECK_EQUAL(t1, timer1);
    BOOST_CHECK_EQUAL(t2, timer2);
    BOOST_CHECK_EQUAL(timerFactory.Calls(), 2);
    BOOST_CHECK_EQUAL(timerFactory.AllTimersUsed(), true);
}

BOOST_AUTO_TEST_CASE(TestNewTimer3)
{
    boost::asio::io_service ioService;
    TimerFactoryMock timerFactory;

    TimerPointer timer1 = TimerPointer(new StandardTimer(ioService));
    TimerPointer timer2 = TimerPointer(new StandardTimer(ioService));
    TimerPointer timer3 = TimerPointer(new StandardTimer(ioService));

    timerFactory.AddTimer(timer1);
    timerFactory.AddTimer(timer2);
    timerFactory.AddTimer(timer3);

    TimerPointer t1 = timerFactory.NewTimer();
    TimerPointer t2 = timerFactory.NewTimer();
    TimerPointer t3 = timerFactory.NewTimer();

    BOOST_CHECK_EQUAL(t1, timer1);
    BOOST_CHECK_EQUAL(t2, timer2);
    BOOST_CHECK_EQUAL(t3, timer3);
    BOOST_CHECK_EQUAL(timerFactory.Calls(), 3);
    BOOST_CHECK_EQUAL(timerFactory.AllTimersUsed(), true);
}

BOOST_AUTO_TEST_CASE(TestNewTimerError1)
{
    TimerFactoryMock timerFactory;

    BOOST_CHECK_THROW(
        timerFactory.NewTimer(),
        AsioExpress::ContractViolationException);
}

BOOST_AUTO_TEST_CASE(TestNewTimerError2)
{
    boost::asio::io_service ioService;
    TimerFactoryMock timerFactory;

    TimerPointer timer1 = TimerPointer(new StandardTimer(ioService));
    TimerPointer timer2 = TimerPointer(new StandardTimer(ioService));

    timerFactory.AddTimer(timer1);
    timerFactory.AddTimer(timer2);

    TimerPointer t1 = timerFactory.NewTimer();
    TimerPointer t2 = timerFactory.NewTimer();

    BOOST_CHECK_THROW(
        timerFactory.NewTimer(),
        AsioExpress::ContractViolationException);
}

BOOST_AUTO_TEST_SUITE_END()
