//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SampleTcpServer1/pch.hpp"

#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"
#include "AsioExpress/Logging/Macros.hpp"
#include "AsioExpress/Yield.hpp" // Enable the pseudo-keywords REENTER, YIELD and FORK.

#include "SampleTcpServer1/MyMessageProcessor.hpp"
#include "SampleTcpServer1/Logger.hpp"

static Logger logger("SampleServer1.MyMessageProcessor", 5);

MyMessageProcessor::MyMessageProcessor(
    AsioExpress::MessagePort::ServerMessage serverMessage,
    int sampleArgument) :
  ServerMessageProcessor(serverMessage),
  m_sampleArgument(sampleArgument),
  m_processorId("MyMessageProcessor")
{
}

void MyMessageProcessor::operator()(AsioExpress::Error error)
{
  STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, this->GetCurrentLine());

  if (error)
  {
    CallCompletionHandler(error);
    STOP_STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, __LINE__);
    return;
  }

  REENTER(this)
  {
      YIELD 
      {
        INFO_MSG(GetIoService(), logger, "Sending message back to sender.");
        AsyncSend(GetMessagePortId(), GetDataBuffer(), *this);
      }

      {
        INFO_MSG(GetIoService(), logger, "Processor Complete.");
        CallCompletionHandler(error);
        STOP_STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, __LINE__);
      }
  }
}
