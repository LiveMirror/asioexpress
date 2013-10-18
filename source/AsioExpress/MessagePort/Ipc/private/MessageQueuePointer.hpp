//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

typedef boost::shared_ptr<boost::interprocess::message_queue> MessageQueuePointer;

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
