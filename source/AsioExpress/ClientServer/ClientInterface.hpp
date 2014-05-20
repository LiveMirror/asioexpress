//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include <string>

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/ClientServer/MessagePortId.hpp"

namespace AsioExpress {
namespace MessagePort {

class ClientInterface
{
public:

  ///
  /// This method attempts to connects the client to the endpoint.
  ///
  virtual void Connect() = 0;

  ///
  /// This method disconnects the client.
  ///
  virtual void Disconnect() = 0;
 
  ///
  /// Disconnects and shutsdown the client. The connect will no longer 
  /// function once the client is shut down.
  ///
  virtual void ShutDown() = 0;

  ///
  /// Send a message to the client.
  ///
  virtual void AsyncSend(
      DataBufferPointer buffer, 
      AsioExpress::CompletionHandler completionHandler) = 0;
  
  /// 
  /// Gets the address of the client system.
  ///
  virtual std::string GetAddress() const = 0;

  virtual ~ClientInterface() {}
};

typedef boost::shared_ptr<ClientInterface> ClientInterfacePointer;

} // namespace MessagePort
} // namespace AsioExpress
