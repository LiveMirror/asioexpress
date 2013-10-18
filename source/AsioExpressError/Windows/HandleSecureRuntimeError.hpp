//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

namespace AsioExpress {

typedef void (*LogRuntimeErrorHandler)(std::string error);
extern void HandleSecureRuntimeError(LogRuntimeErrorHandler handler);

} // namespace AsioExpress
