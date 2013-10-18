//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/MessagePortId.hpp"

namespace AsioExpress {
namespace MessagePort {

class ServerInterface
{
public:
  virtual void Start() = 0;

  virtual void Stop() = 0;

  virtual void GetIds(MessagePortIdList & list) const = 0;

  virtual void AsyncSend(
      MessagePortId id, 
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler) = 0;

  virtual void AsyncBroadcast(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler) = 0;

  virtual ~ServerInterface() {}
};

typedef boost::shared_ptr<ServerInterface> ServerInterfacePointer;

} // namespace MessagePort
} // namespace AsioExpress
