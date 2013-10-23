//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include <cstdarg>
#include <sstream>

#include "AsioExpressError/private/Contract.hpp"
#include "AsioExpress/Platform/DebugMessage.hpp"

using namespace std;

static char const * 
CONTRACT_VIOLATION_MESSAGE = "Component Contract Violation";

static char const * 
CONTRACT_VIOLATION_MESSAGE_DELIMITER = " - ";

static char const * 
CONTRACT_VIOLATION_MESSAGE_FILE_DELIMITER = ":";

#ifdef _MSC_VER
#pragma warning(disable: 4702 )
#endif // _MSC_VER

std::string AsioExpress::errorReport(
        char const * condition, 
        std::string const & message, 
        char const * file, 
        int line)
{
    ostringstream errorMessage;
    
    errorMessage << CONTRACT_VIOLATION_MESSAGE;

    if (! message.empty())
    {
        errorMessage << CONTRACT_VIOLATION_MESSAGE_DELIMITER;
        errorMessage << message;
    }

    if (condition != NULL)
    {
        errorMessage << CONTRACT_VIOLATION_MESSAGE_DELIMITER;
        errorMessage << condition;
    }

    if (file != NULL)
    {
        errorMessage << CONTRACT_VIOLATION_MESSAGE_DELIMITER;
        errorMessage << file << CONTRACT_VIOLATION_MESSAGE_FILE_DELIMITER << line;
    }

    errorMessage << endl;

    return errorMessage.str(); 
}
