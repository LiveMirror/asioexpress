//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/ServerInterface.hpp"

namespace AsioExpress {
namespace MessagePort {

class RoundRobinServer
{
public:
  explicit RoundRobinServer(
      ServerInterfacePointer server);
  
  void AsyncSendRoundRobin(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler);
  
private:
    ServerInterfacePointer  server;
    size_t                  nextClientIndex;
};

} // namespace MessagePort
} // namespace AsioExpress
