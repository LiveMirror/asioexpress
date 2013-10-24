//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressErrorTest/pch.hpp"


#ifdef _MSC_VER
#include "AsioExpressError/Windows/Win32Exception.hpp"
#include "AsioExpressWinStack/CallStackService.h"
#endif

#include "AsioExpress/InstanceManager.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace AsioExpress;

struct GlobalSetup 
{
    GlobalSetup() 
    { 
#ifdef _MSC_VER
      Win32Exception::installHandler();
#endif
    }

    ~GlobalSetup()         
    {
      CLEAN_UP_DEBUG_TIMERS;
      InstanceManager::CleanUp();
    }
};

BOOST_GLOBAL_FIXTURE(GlobalSetup);
