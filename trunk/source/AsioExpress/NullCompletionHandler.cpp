//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"

namespace AsioExpress {

void NullCompletionHandler(AsioExpress::Error)
{
}

} // namespace AsioExpress