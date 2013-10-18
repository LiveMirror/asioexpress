//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

#include "AsioExpress/MessagePort/Tcp/private/MessagePortAcceptor.hpp"
#include "AsioExpress/MessagePort/Tcp/BasicMessagePort.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

typedef MessagePortAcceptor<BasicMessagePort> BasicMessagePortAcceptor;

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
