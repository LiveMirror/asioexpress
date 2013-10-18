//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "AsioExpressErrorTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include <string>

#include "AsioExpressError/ContractViolationException.hpp"
#include "AsioExpressError/Check.hpp"

using namespace AsioExpress;
using namespace std;

// 
// This symbol is only used locally.
// Set to 1 to debug the test code with the debugger.
//
#define DEBUGGER_TEST_MODE 0


#if DEBUGGER_TEST_MODE == 1
#pragma message("ContractTest.h: warning: Unit tests will break out to the debugger. Turn off when finished debugging.")
#endif

BOOST_AUTO_TEST_SUITE(ContractTest)

BOOST_AUTO_TEST_CASE(TestCheckMacro)
{
#if DEBUGGER_TEST_MODE == 1
    // testing various expressions
    {
        int x = 10;
        int y = 10;
        int z = 11;
        string text("hello");

        // The following checks are valid.
        CHECK(x == y);
        CHECK(x != z);
        CHECK(x + 1 == y + 1);
        CHECK(x == 10);
        CHECK(text == "hello");
        CHECK(!text.empty());
        CHECK(x == 10 ? true : false);

        // The following checks are invalid and 
        // should break out to the debugger.
        CHECK(false);
        CHECK(x != y);
        CHECK(x == z);
        CHECK(x + 1 == y + 2);
        CHECK(x == 13);
        CHECK(text == "hi");
        CHECK(text.empty());
        CHECK(x == 13 ? true : false);
    }
#else
    // testing various expressions
    {
        int x = 10;
        int y = 10;
        int z = 11;
        string text("hello");

        BOOST_CHECK_NO_THROW(CHECK(x == y));
        BOOST_CHECK_NO_THROW(CHECK(x != z));
        BOOST_CHECK_NO_THROW(CHECK(x + 1 == y + 1));
        BOOST_CHECK_NO_THROW(CHECK(x == 10));
        BOOST_CHECK_NO_THROW(CHECK(text == "hello"));
        BOOST_CHECK_NO_THROW(CHECK(!text.empty()));
        BOOST_CHECK_NO_THROW(CHECK(x == 10 ? true : false));

        BOOST_CHECK_THROW(CHECK(false), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(x != y), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(x == z), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(x + 1 == y + 2), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(x == 13), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(text == "hi"), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(text.empty()), ContractViolationException);
        BOOST_CHECK_THROW(CHECK(x == 13 ? true : false), ContractViolationException);
    }

    // test for side effects in conditional blocks
    {
        int x = 1;
        try
        {
            if (true)
                CHECK(x == 1);
            else
                BOOST_FAIL("Conditional else block executed as a side effect.");
        }
        catch(...)
        {
            BOOST_FAIL("Invalid exception generated.");
        }
    }

    // test for catching the violation exception
    {
        bool caughtException(false);
        try
        {
            CHECK(false);
        }
        catch(ContractViolationException const & contractViolationException)
        {
            caughtException = true;
            BOOST_CHECK_MESSAGE(
                strlen(contractViolationException.what()),
                "Missing a message for the exception");
        }
        BOOST_CHECK_MESSAGE(
            caughtException,
            "Failed to generate an contract violation exception."); 
    }
#endif
}

BOOST_AUTO_TEST_CASE(TestCheckMsgMacro)
{
#if DEBUGGER_TEST_MODE == 1
    // testing various expressions
    {
        int x = 10;
        int y = 10;
        int z = 11;
        string text("hello");

        // The following checks are valid.
        CHECK_MSG(x == y, "Error message.");
        CHECK_MSG(x != z, "Error message.");
        CHECK_MSG(x + 1 == y + 1, "Error message.");
        CHECK_MSG(x == 10, "Error message.");
        CHECK_MSG(text == "hello", "Error message.");
        CHECK_MSG(!text.empty(), "Error message.");
        CHECK_MSG(x == 10 ? true : false, "Error message.");

        // The following checks are invalid and 
        // should break out to the debugger.
        CHECK_MSG(false, "Error message.");
        CHECK_MSG(x != y, "Error message.");
        CHECK_MSG(x == z, "Error message.");
        CHECK_MSG(x + 1 == y + 2, "Error message.");
        CHECK_MSG(x == 13, "Error message.");
        CHECK_MSG(text == "hi", "Error message.");
        CHECK_MSG(text.empty(), "Error message.");
        CHECK_MSG(x == 13 ? true : false, "Error message.");
    }
#else
    // testing various expressions
    {
        int x = 10;
        int y = 10;
        int z = 11;
        string text("hello");

        BOOST_CHECK_NO_THROW(CHECK_MSG(x == y, "Error message."));
        BOOST_CHECK_NO_THROW(CHECK_MSG(x != z, "Error message."));
        BOOST_CHECK_NO_THROW(CHECK_MSG(x + 1 == y + 1, "Error message.") );
        BOOST_CHECK_NO_THROW(CHECK_MSG(x == 10, "Error message."));
        BOOST_CHECK_NO_THROW(CHECK_MSG(text == "hello", "Error message."));
        BOOST_CHECK_NO_THROW(CHECK_MSG(!text.empty(), "Error message."));
        BOOST_CHECK_NO_THROW(CHECK_MSG(x == 10 ? true : false, "Error message."));

        BOOST_CHECK_THROW(CHECK_MSG(false, "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(x != y, "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(x == z, "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(x + 1 == y + 2, "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(x == 13, "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(text == "hi", "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(text.empty(), "Error message."), ContractViolationException);
        BOOST_CHECK_THROW(CHECK_MSG(x == 13 ? true : false, "Error message."), ContractViolationException);
    }

    // test for side effects in conditional blocks
    {
        int x = 1;
        try
        {
            if (true)
                CHECK_MSG(x == 1, "Error message.");
            else
                BOOST_FAIL("Conditional else block executed as a side effect.");
        }
        catch(...)
        {
            BOOST_FAIL("Invalid exception generated.");
        }
    }

    // test for catching the violation exception
    {
        bool caughtException(false);
        int x = 1;
        try
        {
            CHECK_MSG(x == 2, "Error message.");
        }
        catch(ContractViolationException const & contractViolationException)
        {
            caughtException = true;
            BOOST_CHECK_MESSAGE(
                strstr(contractViolationException.what(), "Error message.") != NULL,
                "Missing a message for the exception");
        }
        BOOST_CHECK_MESSAGE(
            caughtException,
            "Failed to generate an contract violation exception.");
    }
#endif
}

BOOST_AUTO_TEST_CASE(TestCheckFailedMacro)
{
#if DEBUGGER_TEST_MODE == 1
    CHECK_FAILED("Error message.");
#else
    BOOST_CHECK_THROW(CHECK_FAILED("Error message."), ContractViolationException);
#endif
}

#pragma warning(disable:4101)
BOOST_AUTO_TEST_CASE(TestStaticCheckMacro)
{
    int const ARRAY_SIZE = 2;
    int intArray[ARRAY_SIZE];

    // Lets check that our array is the correct size.
    STATIC_CHECK(sizeof(intArray) / sizeof(int) == ARRAY_SIZE);
}

BOOST_AUTO_TEST_SUITE_END()
