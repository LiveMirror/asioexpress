//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "AsioExpress/MessagePort/Tcp/EndPoint.hpp"
#include "AsioExpress/ClientServer/MessagePortClient.hpp"
#include "AsioExpress/MessagePort/Tcp/BasicMessagePort.hpp"
#include "AsioExpress/Timer/StandardTimer.hpp"
#include "AsioExpress/Timer/StandardRepeatingTimer.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"

#include "SampleTcpClient1/Logger.hpp"
#include "SampleTcpClient1/AppErrorHandlerImpl.hpp"
#include "SampleTcpClient1/EventHandler.hpp"

typedef AsioExpress::MessagePort::MessagePortClient<AsioExpress::MessagePort::Tcp::BasicMessagePort> MessagePortClient;

class App
{
public:
  App(
      boost::asio::io_service & ioService,
      const AsioExpress::MessagePort::Tcp::EndPoint& clientEndPoint, 
      const std::string& clientName) :
    m_ioService(ioService),
    m_logger("SampleTcpClient1.App", 5),
    m_errorHandler(new AppErrorHandlerImpl),
    m_endPoint(clientEndPoint),
    m_retryTimer(m_ioService),
    m_sendTimer(m_ioService),
    m_clientName(clientName),
    m_isShutdown(false)
  {
  }

  void Start()
  {
    connect();

    m_sendTimer.AsyncWait(
      500, 
      boost::bind(&App::concurrentSend, this, _1));
  }

  void Stop()
  {
    m_isShutdown = true;
    m_client->ShutDown();
    m_retryTimer.Stop();
    m_sendTimer.Stop();
  }

protected:
  App();

  void reconnect()
  {
    if (!m_isShutdown)
    {
      m_retryTimer.AsyncWait(
        5000,
        boost::bind(&App::connect, this, _1));
    }
  }

  void concurrentSend(AsioExpress::Error error = AsioExpress::Error())
  {
    static int count;

    std::ostringstream msg;
    msg << "Message " << ++count;

    Message message(msg.str());

    INFO_MSG(m_ioService, m_logger, "Sending message: " << message.GetText());

    m_client->AsyncSend(message.GetDataBuffer(), AsioExpress::NullCompletionHandler);
  }

  void connect(AsioExpress::Error error = AsioExpress::Error())
  {
    if (error.GetErrorCode() != boost::asio::error::operation_aborted)
    {
      m_client.reset(new MessagePortClient(
          m_ioService, 
          m_endPoint, 
          new EventHandler(m_errorHandler, m_clientName, boost::bind(&App::reconnect, this))));

      m_client->Connect();
    }
  }

private:
  boost::asio::io_service &                 m_ioService;  
  Logger                                    m_logger;
  AppErrorHandlerPointer                    m_errorHandler;
  AsioExpress::MessagePort::Tcp::EndPoint   m_endPoint;
  AsioExpress::StandardTimer                m_retryTimer;
  AsioExpress::StandardRepeatingTimer       m_sendTimer;
  boost::shared_ptr<MessagePortClient>      m_client;
  std::string                               m_clientName;
  bool                                      m_isShutdown;
};
