//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/MessagePort/Tcp/private/SocketPointer.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

extern void SetSocketOptions(SocketPointer const & socket);

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
