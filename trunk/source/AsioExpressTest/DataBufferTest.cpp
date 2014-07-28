//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"

#include "AsioExpress/MessagePort/DataBuffer.hpp"

using namespace AsioExpress;
using namespace AsioExpress::MessagePort;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(DataBufferTest)

BOOST_AUTO_TEST_CASE(Test_Construction)
{
    DataBuffer buffer(25);

    BOOST_CHECK_EQUAL( buffer.Size(), 25);
    BOOST_CHECK( buffer.Get() != 0 );

    memset(buffer.Get(), 255, 25);
}

BOOST_AUTO_TEST_CASE(Test_Str_Construction)
{
  char const *const text = "foobar";
  std::string str(text);
  DataBuffer buffer(str);
  BOOST_CHECK_EQUAL( buffer.Size(), 6);
  BOOST_CHECK( buffer.Get() != 0 );
  BOOST_CHECK( memcmp(buffer.Get(), text, 6) == 0 );
}

BOOST_AUTO_TEST_CASE(Test_Assign)
{
    char const * const text = "123456789a";

    DataBuffer buffer;

    buffer.Assign(text, 10);

    BOOST_CHECK_EQUAL( buffer.Size(), 10);
    BOOST_CHECK( buffer.Get() != 0 );
    BOOST_CHECK( memcmp(buffer.Get(), text, 10) == 0 );
}


BOOST_AUTO_TEST_CASE(Test_Copy)
{
    DataBuffer b1(25);
    DataBuffer b2(b1);

    BOOST_CHECK_EQUAL( b2.Size(), b1.Size() );
    BOOST_CHECK( b1.Get() != b2.Get() );
    BOOST_CHECK( memcmp(b1.Get(), b2.Get(), b1.Size()) == 0 );
}

BOOST_AUTO_TEST_CASE(Test_Assignment)
{
    DataBuffer b1(25);
    DataBuffer b2;

    b2 = b1;

    BOOST_CHECK_EQUAL( b2.Size(), b1.Size() );
    BOOST_CHECK( b1.Get() != b2.Get() );
    BOOST_CHECK( memcmp(b1.Get(), b2.Get(), b1.Size()) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()
