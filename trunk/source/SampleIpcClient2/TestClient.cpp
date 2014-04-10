//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressConfig/config.hpp"

#include <iostream>
#include <boost/asio.hpp>
#include <csignal>

#include "AsioExpress/Platform/Sleep.hpp"
#include "AsioExpressError/BasicException.hpp"
#include "AsioExpress/MessagePort/SyncIpc/EndPoint.hpp"
#include "AsioExpress/MessagePort/SyncIpc/MessagePort.hpp"

using namespace AsioExpress::MessagePort::SyncIpc;

bool Connect(MessagePort & messagePort)
{
    try
    {
        messagePort.Connect(EndPoint("nowhere"));
    }
    catch(std::exception const &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }

    return true;
}

void SendMessages()
{
    MessagePort messagePort;

    while (! Connect(messagePort) )
    {
        static int connectAttempt;
        if (++connectAttempt == 5)
            throw AsioExpress::BasicException("Cannot connect to server.");

        AsioExpress::Sleep(5000);
    }

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

        AsioExpress::Sleep(1000);
    }

    for (;;)
    {
        AsioExpress::MessagePort::DataBufferPointer dataBuffer;

        messagePort.Receive(dataBuffer);

        std::string msg;
        msg.assign(dataBuffer->Get(), dataBuffer->Size());

        std::cout << "Received: " << msg << "\n";

        std::cout << "Delay...\n";

        AsioExpress::Sleep(1000);
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
