//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace AsioExpress {
namespace MessagePort {

extern long const InvalidMessagePortId;

typedef long MessagePortId;
typedef std::vector<MessagePortId> MessagePortIdList;
typedef boost::shared_ptr<MessagePortIdList> MessagePortIdListPointer;

} // namespace MessagePort
} // namespace AsioExpress
