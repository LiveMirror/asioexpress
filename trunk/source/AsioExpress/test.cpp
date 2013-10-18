//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/pch.hpp"

#include "AsioExpressConfig/config.hpp"

#include <string>

#pragma warning(push)
#pragma warning(disable: 4503)
#include "boost/asio.hpp"
#pragma warning(pop)

#include "AsioExpress/Error.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"
#include "AsioExpress/SimpleStateMachine.hpp"
#include "AsioExpress/MessagePort/Tcp/BasicMessagePort.hpp"
#include "AsioExpress/MessagePort/Tcp/BasicMessagePortAcceptor.hpp"

#include "AsioExpress/ClientServer/ClientConnection.hpp"
#include "AsioExpress/ClientServer/ClientMessage.hpp"
#include "AsioExpress/ClientServer/MessagePortClient.hpp"

#include "AsioExpress/ClientServer/ServerMessage.hpp"
#include "AsioExpress/ClientServer/ServerConnection.hpp"
#include "AsioExpress/ClientServer/MessagePortServer.hpp"

#include "AsioExpress/Timer/StandardTimer.hpp"
#include "AsioExpress/Timer/StandardDateTimer.hpp"
#include "AsioExpress/Timer/StandardRepeatingTimer.hpp"

#include "AsioExpress/EventHandling/UniqueEvent.hpp"
#include "AsioExpress/EventHandling/EventQueue.hpp"
#include "AsioExpress/Testing/TimerMock.hpp"
#include "AsioExpress/EventHandling/ResourceCache.hpp"

#include "AsioExpress/Logging/Macros.hpp"

using namespace AsioExpress::MessagePort;
using namespace AsioExpress::MessagePort::Tcp;
using namespace AsioExpress;
using namespace AsioExpress::Testing;

class MyClientProcessor : 
  private SimpleStateMachine
{
public:
  MyClientProcessor(ClientMessage message)
  {
  }

  void operator()()
  {
    switch(CurrentStep())
    {
    case 1:
      Forward();
      break;
    case 2:
      Back();
      break;
    case 3:
      break;
    }
  }
};

class MyServerProcessor : 
  private SimpleStateMachine
{
public:
  MyServerProcessor(ServerMessage message)
  {
  }

  void operator()()
  {
    switch(CurrentStep())
    {
    case 1:
      Forward();
      break;
    case 2:
      Back();
      break;
    case 3:
      break;
    }
  }
};

class MyClientEventHandler : public ClientEventHandler
{
public:
  void ClientConnected(ClientConnection)
  {
  }

  void ClientDisconnected(ClientConnection, AsioExpress::Error)
  {
  }

  void AsyncProcessMessage(ClientMessage message)
  {
    MyClientProcessor proc(message);
    proc();
  }

  AsioExpress::Error ConnectionError(ClientConnection, AsioExpress::Error error)
  {
    return error;
  }

  AsioExpress::Error MessageError(ClientMessage, AsioExpress::Error error)
  {
    return error;
  }
};

class MyServerEventHandler : public ServerEventHandler
{
public:
  void ClientConnected(ServerConnection)
  {
  }

  void ClientDisconnected(ServerConnection, AsioExpress::Error)
  {
  }

  void AsyncProcessMessage(ServerMessage message)
  {
    MyServerProcessor proc(message);
    proc();
  }

  AsioExpress::Error ConnectionError(ServerConnection, AsioExpress::Error error)
  {
    return error;
  }

  AsioExpress::Error MessageError(ServerMessage, AsioExpress::Error error)
  {
    return error;
  }
};

class MyResourceCache : public ResourceCache<std::string, int>
{
protected:
  virtual void AsyncRemoveItem(
      Key /*key*/,
      ResourceItemPointer /*item*/, 
      CompletionHandler /*completionHandler*/)
  {
  }

  virtual void AsyncUpdateAllItems(
      CacheUpdatePointer /*update*/, 
      CompletionHandler /*completionHandler*/)
  {
  }

  virtual void ErrorHandler(
    AsioExpress::Error /*error*/)
  {
  }
};

class MyLogger 
{
public:
  MyLogger(std::string name, int level=0) :
      m_name(name),
      m_level(level)
  {
  }

  bool isLevelTrace() { return m_level < 1; }
  bool isLevelDebug() { return m_level < 2; }
  bool isLevelInfo() { return m_level < 3; }
  bool isLevelWarn() { return m_level < 4; }
  bool isLevelError() { return m_level < 5; }
  bool isLevelFatal() { return m_level < 6; }

  void WriteTrace(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +TRACE: " << message << " (" << file << ":" << line << ")\n";
  }

  void WriteDebug(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +DEBUG: " << message << " (" << file << ":" << line << ")\n";
  }

  void WriteInfo(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +INFO: " << message << " (" << file << ":" << line << ")\n";
  }

  void WriteWarn(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +WARN: " << message << " (" << file << ":" << line << ")\n";
  }

  void WriteError(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +ERROR: " << message << " (" << file << ":" << line << ")\n";
  }

  void WriteFatal(std::string message, char const *file, int line)
  {
    std::cout << m_name << " +FATAL: " << message << " (" << file << ":" << line << ")\n";
  }

private:
  std::string m_name;
  int         m_level;
};

static MyLogger logger("test");

void func()
{
  BasicMessagePortAcceptor::EndPointType endPoint("10.6.0.204","5555");
  boost::asio::io_service ioService;
  BasicMessagePortAcceptor mpa(ioService, endPoint);
  BasicMessagePort mp(ioService);
  mp.AsyncConnect(endPoint, NullCompletionHandler);

  MessagePortServer<BasicMessagePortAcceptor> mps(ioService, endPoint, new MyServerEventHandler);

  MessagePortClient<BasicMessagePort> mpc(ioService, endPoint, new MyClientEventHandler);

  Error error;

  StandardTimer timer1(ioService);
  StandardDateTimer timer2(ioService);
  StandardRepeatingTimer timer3(ioService);

  typedef UniqueEvent<std::string> TestQueue;

  TestQueue testQueue;
  TestQueue::EventPointer eventData(new std::string);
  TimerMockPointer timerMock(new TimerMock);

  TestQueue::ListenerPointer l = testQueue.NewListener(eventData);

  testQueue.AsyncWait(
    l,
    timerMock,
    NullCompletionHandler);

  testQueue.Add(TestQueue::Event(std::string("hello")));

  timerMock->Expire(__FILE__, __LINE__);

  MyResourceCache resourceCache;
  resourceCache.AsyncUpdate(NullCompletionHandler);

  int logInt = 42;

  LOG_TRACE(ioService,logger,"test " << logInt);
  LOG_TRACE_STR(ioService,logger,"test");
  LOG_TRACE_DIRECT(logger,"test " << logInt);
  LOG_TRACE_STR_DIRECT(logger,"test");

  LOG_DEBUG(ioService,logger,"test " << logInt);
  LOG_DEBUG_STR(ioService,logger,"test");
  LOG_DEBUG_DIRECT(logger,"test " << logInt);
  LOG_DEBUG_STR_DIRECT(logger,"test");

  LOG_INFO(ioService,logger,"test " << logInt);
  LOG_INFO_STR(ioService,logger,"test");
  LOG_INFO_DIRECT(logger,"test " << logInt);
  LOG_INFO_STR_DIRECT(logger,"test");

  LOG_WARN(ioService,logger,"test " << logInt);
  LOG_WARN_STR(ioService,logger,"test");
  LOG_WARN_DIRECT(logger,"test " << logInt);
  LOG_WARN_STR_DIRECT(logger,"test");

  LOG_ERROR(ioService,logger,"test " << logInt);
  LOG_ERROR_STR(ioService,logger,"test");
  LOG_ERROR_DIRECT(logger,"test " << logInt);
  LOG_ERROR_STR_DIRECT(logger,"test");

  LOG_FATAL(ioService,logger,"test " << logInt);
  LOG_FATAL_STR(ioService,logger,"test");
  LOG_FATAL_DIRECT(logger,"test " << logInt);
  LOG_FATAL_STR_DIRECT(logger,"test");
}
