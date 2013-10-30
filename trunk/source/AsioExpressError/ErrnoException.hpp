//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/CommonException.hpp"

namespace AsioExpress {

///
/// Use this exception to throw an error that is based on a generic errno 
/// error code. Describe the operation that failed in the description.
///
class ErrnoException : public CommonException
{
public:
    ///
    /// The constructor requires an error code and a description.
    /// @param errno -- The "errno" error code returned by the failed operation.
    /// @param description -- A text string describing the operation that failed.
    ///
    ErrnoException(int errno, std::string description) :
        CommonException(Error(boost::system::error_code(
                                 errno, 
                                 boost::system::generic_category()),
                              AppendCallStack(description)))
    {
    }

    ///
    /// A virtual destructor makes deleting references to base classes safe.
    ///
    virtual ~ErrnoException() throw ()
    {
    }
};

} // namespace AsioExpress
