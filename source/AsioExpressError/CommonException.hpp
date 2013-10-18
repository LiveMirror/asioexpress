//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <exception>

#include "AsioExpressError/Error.hpp"

namespace AsioExpress {

///
/// This is the common base class that all exceptions are derived.
/// It allows us to get an error object from the exception that may
/// be useful as a return value from threaded operations.
///
class CommonException : public std::exception
{
public:
    ///
    /// The constructor requires an error object describing the exception.
    /// @param error An error object describing the exception thrown.
    ///
    explicit CommonException(Error error);

    ///
    /// A virtual destructor makes deleting references to base classes safe.
    ///
    virtual ~CommonException()
    {
    }

    ///
    /// Returns a text description of the exception.
    ///
    virtual char const * what() const;

    ///
    /// Returns a text description of the exception.
    ///
    Error GetError() const;

protected:
    CommonException & operator=(CommonException const &);

    Error error;
};

} // namespace AsioExpress
