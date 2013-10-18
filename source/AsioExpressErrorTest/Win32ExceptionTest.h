//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "AsioExpressError/Win32Exception.hpp"

class Win32ExceptionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Win32ExceptionTest);
    CPPUNIT_TEST(testCatch);
    CPPUNIT_TEST(testWhatType);
    CPPUNIT_TEST(testWhatCallStack);
    CPPUNIT_TEST(testWhere);
    CPPUNIT_TEST(testCode);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

private:
    void testCatch();
    void testWhatType();
    void testWhatCallStack();
    void testWhere();
    void testCode();
};
