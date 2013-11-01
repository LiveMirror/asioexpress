//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/asio.hpp>
#include <csignal>

#include "AsioExpress/MessagePort/SyncIpc/SyncIpcEndPoint.hpp"
#include "AsioExpress/MessagePort/SyncIpc/SyncIpcMessagePort.hpp"

void SendMessages()
{
    using namespace AsioExpress::MessagePort::SyncIpc;

    SyncIpcMessagePort messagePort;

    messagePort.Connect(SyncIpcEndPoint("nowhere"));
    
    for (int i = 0; i < 10; ++i)
    {
        std::ostringstream message;
        message 
          << "Message " 
          << i + 1;

        AsioExpress::MessagePort::DataBufferPointer dataBuffer(
          new AsioExpress::MessagePort::DataBuffer(message.str().size()));
        memcpy(dataBuffer->Get(), message.str().c_str(), message.str().size());
        
        std::cout << "Sending: " << message.str() << "\n";

        messagePort.Send(dataBuffer);
        
        messagePort.Receive(dataBuffer);
        
        std::string msg;
        msg.assign(dataBuffer->Get(), dataBuffer->Size());
        
        std::cout << "Received: " << msg << "\n";
        
        std::cout << "Delay...\n";
        
        sleep(1);
    }    
}

int main(int argc, char* argv[])
{
    try
    {
        SendMessages();
    }
    catch(std::exception const &e)
    {
        std::cout << e.what();
    }
  
	return 0;
}
