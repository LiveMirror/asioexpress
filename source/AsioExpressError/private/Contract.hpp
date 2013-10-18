//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

namespace AsioExpress {

///
/// This is the function used to report a contract error.
/// @params condition The boolean expression that indicates a 
/// contract failure has occured. This value may be set null.
/// @params message A message explaining the failure. This value
/// may be an empty string.
/// @params file The file where the error occurred.
/// @params line The line where the error occurred.
/// @return Return true to invoke a user breakpoint.
///
extern std::string errorReport(
        char const * condition, 
        std::string const & message, 
        char const * file, 
        int line);

} // namespace AsioExpress
