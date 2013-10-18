//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/MessagePort/Databuffer.hpp"
#include "AsioExpress/ClientServer/ServerMessageProcessor.hpp"
#include "AsioExpress/UniqueId.hpp"

#include "SampleTcpServer1/BasicServer.hpp"

class MyMessageProcessor : 
  private AsioExpress::Coroutine,
  public AsioExpress::MessagePort::ServerMessageProcessor
{
public:
  MyMessageProcessor(
      AsioExpress::MessagePort::ServerMessage serverMessage,
      int sampleArgument);

  virtual void operator()(AsioExpress::Error error = AsioExpress::Error());

private:
  int                     m_sampleArgument;
  AsioExpress::UniqueId   m_processorId;
};
