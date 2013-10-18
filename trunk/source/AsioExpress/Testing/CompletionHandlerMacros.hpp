//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>
#include "cxxtest/TestSuite.h"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpressError/Check.hpp"
#include <boost/asio.hpp> 

namespace AsioExpress {
namespace Testing {

inline void CallCompletionHandler(   
  AsioExpress::CompletionHandler completionHandler,
  AsioExpress::Error error)
{
  completionHandler(error);
}

} // namespace Testing
} // namespace AsioExpress

#define BEGIN_MOCK_ASYNC_FUNC boost::function<void (
#define END_MOCK_ASYNC_FUNC(name, paramNumber) )> name = boost::bind(AsioExpress::Testing::CallCompletionHandler, paramNumber, AsioExpress::Error());

#define BEGIN_MOCK_ASYNC_FUNC_WITH_ERROR boost::function<void (
#define END_MOCK_ASYNC_FUNC_WITH_ERROR(name, paramNumber, error) )> name = boost::bind(AsioExpress::Testing::CallCompletionHandler, paramNumber, error);
