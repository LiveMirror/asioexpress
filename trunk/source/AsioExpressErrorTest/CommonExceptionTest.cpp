//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "AsioExpressErrorTest/pch.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/system/error_code.hpp>

#include <string>

#include "AsioExpressError/BasicException.hpp"
#include "AsioExpressError/CommonException.hpp"
#include "AsioExpressError/ErrnoException.hpp"

using namespace AsioExpress;
using namespace std;


BOOST_AUTO_TEST_SUITE(CommonExceptionTest)

class MyException
{
public:
    std::string Message() const
    {
        return "My exception";
    }
};

BOOST_AUTO_TEST_CASE(TestGenericError1)
{
    try
    {
        throw CommonException(
                Error(
                    boost::system::error_code(
                        2, 
                        boost::system::generic_category()),
                    "My file operation failed."));
        throw 25;
    }
    catch(AsioExpress::CommonException const & e)
    {
        string message = e.GetError().Message();
        cout << message << "\n";
    }
    catch(std::exception & e)
    {
        string message(e.what());
        cout << message << "\n";
    }
    catch(...)
    {
        std::cout << "Unknown Exception\n";
    }
}

BOOST_AUTO_TEST_CASE(TestGenericError2)
{
    try
    {
        throw CommonException(
                Error(
                    boost::system::error_code(
                        2, 
                        boost::system::generic_category()),
                    "My file operation failed."));
        throw 25;
    }
    catch(AsioExpress::CommonException const & e)
    {
        string message = e.GetError().Message();
        cout << message << "\n";
    }
    catch(std::exception & e)
    {
        string message(e.what());
        cout << message << "\n";
    }
    catch(...)
    {
        std::cout << "Unknown Exception\n";
    }
}

BOOST_AUTO_TEST_SUITE_END()
