//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "windows.h"

#include <sstream>

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/Windows/WinCallStack.hpp"
#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpressError/Windows/Win32Exception.hpp"

using namespace AsioExpress;

static char const *getShortDescription(unsigned exceptionCode)
{
    switch (exceptionCode) 
    {
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            return "Datatype misalignment";

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            return "Array bounds exceeded";

        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return "Floating point denormal operand";

        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            return "Floating point divide by zero";

        case EXCEPTION_FLT_INEXACT_RESULT:
            return "Floating point inexact result";

        case EXCEPTION_FLT_INVALID_OPERATION:
            return "Floating point invalid operation";

        case EXCEPTION_FLT_OVERFLOW:
            return "Floating point overflow";

        case EXCEPTION_FLT_STACK_CHECK:
            return "Floating point stack check";

        case EXCEPTION_FLT_UNDERFLOW:
            return "Floating point underflow";

        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            return "Integer divide by zero";

        case EXCEPTION_INT_OVERFLOW:
            return "Integer overflow";

        case EXCEPTION_PRIV_INSTRUCTION:
            return "Priviledged instruction";

        case EXCEPTION_IN_PAGE_ERROR:
            return "In page error";

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            return "Illegal instruction";

        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return "Noncontinuable exception";

        case EXCEPTION_STACK_OVERFLOW:
            return "Stack overflow";

        case EXCEPTION_INVALID_DISPOSITION:
            return "Invalid exception disposition";

        case EXCEPTION_GUARD_PAGE:
            return "Guard page";

        case EXCEPTION_INVALID_HANDLE:
            return "Invalid handle";

        case CONTROL_C_EXIT:
            return "Control-C";

        case STATUS_NO_MEMORY:
            return "Not enough quota";

        case STATUS_DLL_NOT_FOUND:
            return "Unable to locate DLL";

        case STATUS_ORDINAL_NOT_FOUND:
            return "DLL ordinal not found";

        case STATUS_ENTRYPOINT_NOT_FOUND:
            return "DLL entry point not found";

        case STATUS_DLL_INIT_FAILED:
            return "DLL initialization failed";
    }

    return NULL;
}

void AsioExpress::Win32Exception::installHandler()
{
    _set_se_translator(Win32Exception::translate);
}

void AsioExpress::Win32Exception::translate(unsigned, EXCEPTION_POINTERS* exceptionInformation)
{
    // Windows guarantees that *(info->ExceptionRecord) is valid
    throw Win32Exception(
      exceptionInformation->ExceptionRecord->ExceptionCode,
      exceptionInformation->ExceptionRecord->ExceptionAddress,
      getDescription(exceptionInformation));
}

std::string AsioExpress::Win32Exception::getDescription(
    EXCEPTION_POINTERS const * exceptionInformation)
{
    unsigned int exceptionCode 
      = exceptionInformation->ExceptionRecord->ExceptionCode;

    AsioExpress::Win32Exception::Address address 
      = exceptionInformation->ExceptionRecord->ExceptionAddress;

    EXCEPTION_RECORD *record = exceptionInformation->ExceptionRecord;

    std::ostringstream messageStream;

    char const *description = getShortDescription(exceptionCode);
    if (description != NULL)
    {
        messageStream 
            << description << " exception at 0x" << std::hex << address << "\n";
    }

    else if (exceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        bool isWrite = record->ExceptionInformation[0] == 1;
        Win32Exception::Address badAddress 
            = reinterpret_cast<Win32Exception::Address>(record->ExceptionInformation[1]);

        messageStream 
            << "Access violation at 0x" << std::hex << address << ": "
            << ": Bad " << (isWrite ? "write" : "read")
            << " on " << badAddress << "\n";
    }

    else
    {
        messageStream 
            << "Win32 exception 0x" << std::hex << exceptionCode 
            << " at 0x" << std::hex << address << "\n";
    }

    messageStream 
      << "Call Stack:\n" 
      << GetWinCallStack(GetCurrentThread(), exceptionInformation->ContextRecord);

    return messageStream.str();
}

AsioExpress::Win32Exception::Win32Exception(
    unsigned int exceptionCode,
    Address address, 
    std::string description) :
  CommonException(Error(Common::ErrorCode::Win32Exception, description)),
  exceptionCode(exceptionCode),
  address(address)
{
}
