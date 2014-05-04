//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <csignal>

#include "AsioExpress/Platform/Sleep.hpp"
#include "AsioExpressError/BasicException.hpp"
#include "AsioExpress/MessagePort/SyncIpc/EndPoint.hpp"
#include "AsioExpress/MessagePort/SyncIpc/MessagePort.hpp"

using namespace AsioExpress::MessagePort::SyncIpc;

void Connect(MessagePort & messagePort)
{
    try
    {
        messagePort.Connect(EndPoint("nowhere"));
    }
    catch(std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Disconnect(MessagePort & messagePort)
{
    try
    {
        messagePort.Disconnect();
    }
    catch(std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void ConnectThread(MessagePort & messagePort)
{
    for (;;)
    {
        if (! messagePort.IsConnected())
        {
            std::cout << "Connect\n";
            Connect(messagePort);
        }

        AsioExpress::Sleep(2000);
    }
}

void DisconnectThread(MessagePort & messagePort)
{
    for (;;)
    {
        if (messagePort.IsConnected())
        {
            std::cout << "Disconnect\n";
            Disconnect(messagePort);
        }

        AsioExpress::Sleep(4000);
    }
}

void SendThread(MessagePort & messagePort)
{
    using namespace AsioExpress::MessagePort;

    static int count;

    for (;;)
    {
        ++count;

        std::ostringstream message;
        message
          << "Message "
          << count;

        DataBufferPointer dataBuffer(new DataBuffer(message.str().size()));
        memcpy(dataBuffer->Get(), message.str().c_str(), message.str().size());

        std::cout << "Sending: " << message.str() << "\n";

        try
        {
            messagePort.Send(dataBuffer);
        }
        catch(std::exception const &e)
        {
            std::cout << e.what() << std::endl;
        }

        AsioExpress::Sleep(2);
    }
}

void ReceiveThread(MessagePort & messagePort)
{
    using namespace AsioExpress::MessagePort;
    for (;;)
    {
        DataBufferPointer dataBuffer(new DataBuffer);

        try
        {
            std::cout << "Receive\n";
            messagePort.Receive(dataBuffer);

            std::string msg;
            msg.assign(dataBuffer->Get(), dataBuffer->Size());

            std::cout << "Received: " << msg << "\n";
        }
        catch(std::exception const &e)
        {
            std::cout << e.what() << std::endl;
        }

        AsioExpress::Sleep(2);
    }
}

int main(int argc, char* argv[])
{
    MessagePort messagePort;

    boost::thread connect(boost::bind(&ConnectThread, boost::ref(messagePort)));
    boost::thread disconnect(boost::bind(&DisconnectThread, boost::ref(messagePort)));
    boost::thread send1(boost::bind(&SendThread, boost::ref(messagePort)));
    boost::thread send2(boost::bind(&SendThread, boost::ref(messagePort)));
    boost::thread recv1(boost::bind(&ReceiveThread, boost::ref(messagePort)));
    boost::thread recv2(boost::bind(&ReceiveThread, boost::ref(messagePort)));

    connect.join();
    disconnect.join();

    return 0;
}
