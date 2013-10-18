//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPointer;

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
