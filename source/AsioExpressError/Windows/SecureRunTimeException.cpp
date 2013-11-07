//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpressError/Windows/SecureRunTimeException.hpp"

AsioExpress::SecureRunTimeException::SecureRunTimeException(
    std::string description) :
  CommonException(Error(Common::ErrorCode::SecureRunTimeException, AppendCallStack(description)))
{
}
