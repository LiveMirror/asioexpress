//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <iostream>

namespace AsioExpress {

void DebugMessage(char const * const message)
{
    std::cout << message;
}

} // namespace AsioExpress
