//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpressError/ContractViolationException.hpp"

AsioExpress::ContractViolationException::ContractViolationException(
    std::string const & description) :
  CommonException(Error(CommonErrorCode::ContractViolation, AppendCallStack(description)))
{
}

AsioExpress::ContractViolationException::~ContractViolationException() throw ()
{
}
