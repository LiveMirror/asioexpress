//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/ClientServer/RoundRobinServer.hpp"

namespace AsioExpress {
namespace MessagePort {

RoundRobinServer::RoundRobinServer(
        ServerInterfacePointer server) :
    server(server),
    nextClientIndex(0)    
{
}
  
void RoundRobinServer::AsyncSendRoundRobin(
    DataBufferPointer buffer, 
    AsioExpress::CompletionHandler completionHandler)
{
    MessagePortIdList messagePortIds;
    server->GetIds(messagePortIds);

    // we will use this invalid id if there are no clients to send to.
    MessagePortId id(1);
    
    if (messagePortIds.size() != 0)
    {
        if (nextClientIndex >= messagePortIds.size())
            nextClientIndex = 0;        
        id = messagePortIds.at(nextClientIndex);
    }
        
    ++nextClientIndex;
    
    server->AsyncSend(
      id, 
      buffer, 
      completionHandler);    
}  

} // namespace MessagePort
} // namespace AsioExpress
