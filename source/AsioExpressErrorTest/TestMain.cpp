//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressErrorTest/pch.hpp"

#include "AsioExpressError/Windows/Win32Exception.hpp"
#include "AsioExpressWinStack/CallStackService.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace AsioExpress;

struct GlobalSetup 
{
    GlobalSetup() 
    { 
      Win32Exception::installHandler();
      InitializeCallStackService();
      StartCallStackService();
    }

    ~GlobalSetup()         
    { 
      StopCallStackService();
    }
};

BOOST_GLOBAL_FIXTURE(GlobalSetup);
