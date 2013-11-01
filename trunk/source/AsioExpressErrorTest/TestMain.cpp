//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressErrorTest/pch.hpp"

#ifdef _MSC_VER
#include "AsioExpressError/Windows/Win32Exception.hpp"
#include "AsioExpressWinStack/CallStackService.h"
#endif // _MSC_VER

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

struct GlobalSetup 
{
    GlobalSetup() 
    { 
#ifdef _MSC_VER
        using namespace AsioExpress;
        Win32Exception::installHandler();
        InitializeCallStackService();
        StartCallStackService();
#endif // _MSC_VER
    }

    ~GlobalSetup()         
    { 
#ifdef _MSC_VER
        using namespace AsioExpress;
        StopCallStackService();
#endif // _MSC_VER
    }
};

BOOST_GLOBAL_FIXTURE(GlobalSetup);
