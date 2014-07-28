#include <boost/test/unit_test.hpp>

#include "AsioExpressTest/pch.hpp"
#include "AsioExpress/Testing/AutoCompletionHandler.hpp"
#include "AsioExpress/Testing/TestCompletionHandler.hpp"
#include "AsioExpress/Testing/DoNotDelete.hpp"
#include <AsioExpress/Testing/hippomocks.h>
#include <AsioExpress/Testing/HippoMockExtensions.hpp>

#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"
#include "AsioExpress/MessagePort/Ipc/private/IpcSendThread.hpp"
#include "AsioExpress/MessagePort/SyncIpc/MessagePort.hpp"

using namespace std;
using namespace AsioExpress;
using namespace AsioExpress::MessagePort;
using namespace AsioExpress::MessagePort::Ipc;
using namespace AsioExpress::MessagePort::SyncIpc;

namespace
{
struct Setup
{
  Setup()
  {
    SetUnitTestMode(true);
    // set max message size to 10 for queue
    sendMessageQueuePointer.reset(
        new boost::interprocess::message_queue(boost::interprocess::create_only,
            "MessagePortsTestQueue", 100, 10));
  }

  ~Setup()
  {
    sendMessageQueuePointer->remove("MessagePortsTestQueue");
  }

  boost::asio::io_service ioService;
  MessageQueuePointer sendMessageQueuePointer;
};
}

BOOST_FIXTURE_TEST_SUITE(MessagePortTest, Setup)

BOOST_AUTO_TEST_CASE(Test_Ipc_Send_Thread_Message_Normal_Size)
{

  IpcSendThreadPointer ipcSendThreadPointer(
      new IpcSendThread(ioService, sendMessageQueuePointer,
          IpcSendThread::EnablePing));

  AsioExpress::Testing::AutoCompletionHandler successCompletionHandler(__FILE__,
  __LINE__);
  // test send message with size 9
  AsioExpress::MessagePort::DataBufferPointer dataBufferPointer(
      new DataBuffer("123456789"));
  ipcSendThreadPointer->TestSend(dataBufferPointer, successCompletionHandler);

  BOOST_CHECK_EQUAL(successCompletionHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(sendMessageQueuePointer->get_num_msg(), 1);
}

BOOST_AUTO_TEST_CASE(Test_Ipc_Send_Thread_Message_Too_Large)
{

  IpcSendThreadPointer ipcSendThreadPointer(
      new IpcSendThread(ioService, sendMessageQueuePointer,
          IpcSendThread::EnablePing));

  AsioExpress::Testing::TestCompletionHandler failureCompletionHandler;

  // test send message with size 11
  AsioExpress::MessagePort::DataBufferPointer dataBuffer(
      new DataBuffer("12345678901"));
  ipcSendThreadPointer->TestSend(dataBuffer, failureCompletionHandler);

  BOOST_CHECK_EQUAL(failureCompletionHandler.Calls(), 1);
  BOOST_CHECK_EQUAL(failureCompletionHandler.LastError().GetErrorCode(),
      ErrorCode::MessageQueueSendFailed);
  std::string errorMessage(failureCompletionHandler.LastError().Message());
  std::string expectedErrorMessage(
      "Message size 11 greater than maximum allowed message size 10");
  BOOST_CHECK_MESSAGE(
      errorMessage.find(expectedErrorMessage) != std::string::npos,
      "\"" << expectedErrorMessage << "\" not in \"" << errorMessage << "\"");
  BOOST_CHECK_EQUAL(sendMessageQueuePointer->get_num_msg(), 0);
}

BOOST_AUTO_TEST_CASE(Test_SyncIpc_Send_Message_Normal)
{
  SyncIpc::MessagePort messagePort;

  // test send message with size 9
  AsioExpress::MessagePort::DataBufferPointer dataBufferPointer(
      new DataBuffer("123456789"));
  BOOST_CHECK_NO_THROW(
      messagePort.TestSend(dataBufferPointer, sendMessageQueuePointer));
  BOOST_CHECK_EQUAL(sendMessageQueuePointer->get_num_msg(), 1);
}

bool IsMessageTooLargeErrorMessage(AsioExpress::CommonException const &ex)
{
  std::string expectedErrorMessage(
      "Message size 11 greater than maximum allowed message size 10");
  std::string errorMessage(ex.GetError().Message());
  return errorMessage.find(expectedErrorMessage) != std::string::npos;
}

BOOST_AUTO_TEST_CASE(Test_SyncIpc_Send_Message_Too_Large)
{
  SyncIpc::MessagePort messagePort;

  // test send message with size 11
  AsioExpress::MessagePort::DataBufferPointer dataBufferPointer(
      new DataBuffer("12345678901"));
  BOOST_CHECK_EXCEPTION(
      messagePort.TestSend(dataBufferPointer, sendMessageQueuePointer),
      AsioExpress::CommonException, IsMessageTooLargeErrorMessage);
  BOOST_CHECK_EQUAL(sendMessageQueuePointer->get_num_msg(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
