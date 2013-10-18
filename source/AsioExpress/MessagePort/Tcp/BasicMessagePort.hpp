//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/MessagePort/Tcp/private/MessagePort.hpp"
#include "AsioExpress/MessagePort/Tcp/private/BasicProtocolSender.hpp"
#include "AsioExpress/MessagePort/Tcp/private/BasicProtocolReceiver.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

typedef MessagePort<BasicProtocolSender, BasicProtocolReceiver> BasicMessagePort;

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
