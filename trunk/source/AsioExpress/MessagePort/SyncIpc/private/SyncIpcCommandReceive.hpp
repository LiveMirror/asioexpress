//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/thread.hpp>

#include "AsioExpress/MessagePort/SyncIpc/private/MessageQueuePointer.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace SyncIpc {

bool SyncIpcCommandReceive(
    MessageQueuePointer recvMessageQueue,
    DataBufferPointer dataBuffer,
    int maxMilliseconds);

bool SyncIpcCommandReceive(
    MessageQueuePointer recvMessageQueue,
    MessageQueuePointer sendMessageQueue,
    boost::mutex & recvMutex,
    boost::mutex & sendMutex,
    DataBufferPointer dataBuffer,
    int maxMilliseconds);

} // namespace SyncIpc
} // namespace MessagePort
} // namespace AsioExpress
