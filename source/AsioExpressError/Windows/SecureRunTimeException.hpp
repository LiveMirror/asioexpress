//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include "AsioExpressError/CommonException.hpp"

namespace AsioExpress {

///
/// This is a basic exception class that records an error message
/// and captures a stack trace.
///
class SecureRunTimeException : public CommonException
{
public:
    ///
    /// The constructor requires a description of the exception.
    /// @param description A text string descriibing the exception thrown.
    ///
    explicit SecureRunTimeException(std::string description);

    ///
    /// A virtual destructor makes deleting references to base classes safe.
    ///
    virtual ~SecureRunTimeException()
    {
    }
};

} // namespace AsioExpress
