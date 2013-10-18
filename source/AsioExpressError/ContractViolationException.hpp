//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include "AsioExpressError/CommonException.hpp"

namespace AsioExpress {

/// This exception indicates that a condition was encountered that
/// violates the usage contract for a class or method and the execution
/// can no longer continue. It indicates a programming error that must 
/// be fixed. It can be caused by incorrect parameters for a method or
/// if a class finds itself in an invalid state.
class ContractViolationException : public CommonException
{
public:
    ContractViolationException(std::string const & message);
    virtual ~ContractViolationException();

private:
    ContractViolationException();
};

} // namespace AsioExpress
