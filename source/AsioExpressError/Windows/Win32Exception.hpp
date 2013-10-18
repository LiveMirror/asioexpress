//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once


#include <string>
#include <windows.h>
#include "AsioExpressError/CommonException.hpp"

namespace AsioExpress {

///
/// Use this class to catch Win32 exceptions. Note that you must call 
/// installHandler() for every thread where you wish to catch this
/// Win32Exception. Use the /EHa compiler flag to enable asyncronous
/// exception handling if you do not want to leak memory when a Win32
/// exception is thrown. Note Win32 excections do not use the standard 
/// C++ exception handling mechanism.
///
class Win32Exception : public CommonException
{
public:
    typedef void const * Address;

    ///
    /// A virtual destructor makes deleting references to base classes safe.
    ///
    virtual ~Win32Exception()
    {
    }

    ///
    /// You must call this initialization method to be able to catch 
    /// exceptions of this type in your code. 
    ///
    static void installHandler();

    ///
    /// Returns the address where the exception occured.
    ///
    Address where() const 
    { 
        return address; 
    }

    ///
    /// Returns the Win32 exception code that was thrown.
    ///
    unsigned int code() const 
    { 
        return exceptionCode; 
    }

protected:
    Win32Exception(
        unsigned int exceptionCode, 
        Address address, 
        std::string description);

    static std::string getDescription(EXCEPTION_POINTERS const * exceptionInformation);

    static void translate(unsigned code, EXCEPTION_POINTERS* info);

private:
    Address         address;
    unsigned int    exceptionCode;
};

} // namespace AsioExpress
