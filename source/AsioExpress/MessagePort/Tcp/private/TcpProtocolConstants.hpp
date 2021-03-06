//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace AsioExpress {
namespace MessagePort {
namespace Tcp {

typedef int ProtocolVersionType;

extern char const *   ProtocolHeaderText;

enum 
{ 
  ProtocolHeaderSize = 8 
};

enum 
{
  ProtocolVersionBasic = 1,
};

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
