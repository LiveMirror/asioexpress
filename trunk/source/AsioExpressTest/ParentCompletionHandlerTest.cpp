//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"

#include "AsioExpress/ParentCompletionHandler.hpp"

using namespace AsioExpress;
using namespace AsioExpress::Testing;
using namespace std;

BOOST_AUTO_TEST_SUITE(ParentCompletionHandlerTest)

BOOST_AUTO_TEST_CASE(TestNoCalls)
{
    AutoCompletionHandler completionHandler(__FILE__, __LINE__);

    // lifetime
    {
        ParentCompletionHandler parent(completionHandler);
        parent.Complete();
    }

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestOneCall)
{
    AutoCompletionHandler completionHandler(__FILE__, __LINE__);

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestTwoCalls)
{
    AutoCompletionHandler completionHandler(__FILE__, __LINE__);

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    CompletionHandler h2(parent.NewChild());
    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h2(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestThreeCalls)
{
    AutoCompletionHandler completionHandler(__FILE__, __LINE__);

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    CompletionHandler h2(parent.NewChild());
    CompletionHandler h3(parent.NewChild());
    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h2(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h3(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestEarlyCompletion)
{
    AutoCompletionHandler completionHandler(__FILE__, __LINE__);

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    CompletionHandler h2(parent.NewChild());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h2(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
}

BOOST_AUTO_TEST_CASE(TestOneError)
{
    TestCompletionHandler completionHandler;

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    CompletionHandler h2(parent.NewChild());
    CompletionHandler h3(parent.NewChild());
    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h2(Error(AsioExpress::ErrorCode::EventQueueTimeout));

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h3(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
    BOOST_CHECK(completionHandler.LastError().GetErrorCode() ==
                AsioExpress::ErrorCode::EventQueueTimeout);
}

BOOST_AUTO_TEST_CASE(TestTwoErrors)
{
    TestCompletionHandler completionHandler;

    ParentCompletionHandler parent(completionHandler);

    CompletionHandler h1(parent.NewChild());
    CompletionHandler h2(parent.NewChild());
    CompletionHandler h3(parent.NewChild());
    parent.Complete();

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h1(Error(AsioExpress::ErrorCode::UniqueEventTimeout));

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h2(Error(AsioExpress::ErrorCode::EventQueueTimeout));

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 0);

    h3(Error());

    BOOST_CHECK_EQUAL(completionHandler.Calls(), 1);
    BOOST_CHECK(completionHandler.LastError().GetErrorCode() ==
                AsioExpress::ErrorCode::EventQueueTimeout);
}

BOOST_AUTO_TEST_SUITE_END()
