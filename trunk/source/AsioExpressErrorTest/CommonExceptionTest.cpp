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

BOOST_AUTO_TEST_CASE(TestGenericError)
{
    try
    {
        //throw MyException();
        
        throw Error(
                boost::system::error_code(
                  2, 
                  boost::system::generic_category()),
                "My file operation failed.");

        throw CommonException(
                Error(
                    boost::system::error_code(
                        2, 
                        boost::system::generic_category()),
                    "My file operation failed."));
        throw 25;
    }
    catch (MyException const & e)
    {
        string message = e.Message();                
    }
    catch (Error const & e)
    {
        string message = e.Message();        
    }
    catch (int n)
    {
        exit(n);
    }
    catch(AsioExpress::CommonException & e)
    {
        string message = e.GetError().Message();
    }
    catch(std::exception & e)
    {
        string message(e.what());
    }
    catch(...)
    {
        std::cout << "Unknown Exception\n";
    }
}

BOOST_AUTO_TEST_SUITE_END()
