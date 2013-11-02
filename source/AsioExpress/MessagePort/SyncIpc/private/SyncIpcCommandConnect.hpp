//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include "AsioExpress/MessagePort/SyncIpc/MessagePort.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

void MessagePortCommandConnect(
        EndPoint const & endPoint, 
        MessagePort & messagePort);

} // namespace SyncIpc
} // namespace MessagePort
} // namespace AsioExpress
