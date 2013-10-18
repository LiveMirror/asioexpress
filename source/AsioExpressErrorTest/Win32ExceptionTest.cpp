//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressErrorTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include <string>

#include "AsioExpressError/Windows/Win32Exception.hpp"
#include "AsioExpressWinStack/CallStackService.h"

using namespace std;
using namespace AsioExpress;

namespace
{
    volatile int Zero = 0;
    volatile int SomeValue = 768;

    class ClassA
    {
    public:
        virtual void CalculateValue()
        {
        }
    };

    class ClassB : public ClassA
    {
    public:
        ClassB() :
            value(SomeValue)
        {
        }

        virtual void CalculateValue()
        {
            value /= Zero;
        }

    private:
        int value;
    };

    ClassB *b;
}

struct Setup 
{
    Setup() : b( 0 ) 
    { 
      b = new ClassB;
    }

    ~Setup()         
    { 
      delete b;
      b = 0;
    }

    ClassB * b;
};

BOOST_FIXTURE_TEST_SUITE( Win32ExceptionTest, Setup )

BOOST_AUTO_TEST_CASE(TestCatch)
{
    BOOST_CHECK_THROW(b->CalculateValue(), Win32Exception); 
}

BOOST_AUTO_TEST_CASE(TestWhatType)
{
    string message;

    try
    {
        b->CalculateValue();
    }
    catch(Win32Exception e)
    {
        message = e.what();
    }

    BOOST_CHECK(message.find("Integer divide by zero") != string::npos);
}

BOOST_AUTO_TEST_CASE(TestWhatCallStack)
{
    string message;

    try
    {
        b->CalculateValue();
    }
    catch(Win32Exception e)
    {
        message = e.what();
    }

    BOOST_CHECK(message.find("ClassB::CalculateValue") != string::npos);
}

BOOST_AUTO_TEST_CASE(TestWhere)
{
    Win32Exception::Address address;
    try
    {
        b->CalculateValue();
    }
    catch(Win32Exception e)
    {
        address = e.where();
    }

    BOOST_CHECK(address != NULL);
}

BOOST_AUTO_TEST_CASE(TestCode)
{
    unsigned int code;
    try
    {
        b->CalculateValue();
    }
    catch(Win32Exception e)
    {
        code = e.code();
    }

    BOOST_CHECK(code == 0xC0000094);
}

BOOST_AUTO_TEST_SUITE_END()
