//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include "AsioExpress/Proc/RunProc.hpp"
#include "AsioExpress/Yield.hpp"

#include "SampleTcpServer1/EchoManager.hpp"
#include "SampleTcpServer1/Logger.hpp"

static Logger logger("SampleServer1.EchoManager", 5);

static void EchoProcessor(
    AsioExpress::Proc::Context context,
    AsioExpress::MessagePort::ServerMessage message, 
    int sampleArgument)
{
  REENTER(context.Coroutine())
  {
    YIELD
    {
      INFO_STR(message.GetIoService(), logger, "Sending message back to sender.");
      message.AsyncSend(
        message.GetMessagePortId(), 
        message.GetDataBuffer(),
        PushProc(&EchoProcessor, context, message, sampleArgument) );
    }

    // Do more async calls.
    //YIELD
    //{
    //}

    INFO_STR(message.GetIoService(), logger, "Processor Complete.");
    context.Complete();
  }
}

void EchoManager::AsyncEcho(
    AsioExpress::MessagePort::ServerMessage message, 
    int sampleArgument,
    AsioExpress::CompletionHandler completionHandler)
{
    RunProc(
      &EchoProcessor,
      AsioExpress::Proc::Context(message.GetIoService(), __FUNCTION__, completionHandler),
      message,
      sampleArgument);
}
