#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>

#include "AsioExpress/MessagePort/Ipc/EndPoint.hpp"

BOOST_AUTO_TEST_SUITE(IpcEndPointTest)

BOOST_AUTO_TEST_CASE(Test_Equality)
{
  using namespace AsioExpress::MessagePort::Ipc;

  boost::interprocess::permissions perm1(0700);
  boost::interprocess::permissions perm2(0644);

  EndPoint endPoint1("MessagePortName1", 1000, 8000, perm1);
  EndPoint endPoint2("MessagePortName1", 1000, 8000, perm1);

  EndPoint endPoint3("MessagePortName2", 1000, 8000, perm1);
  EndPoint endPoint4("MessagePortName1", 2000, 8000, perm1);
  EndPoint endPoint5("MessagePortName1", 1000, 9000, perm1);
  EndPoint endPoint6("MessagePortName1", 1000, 8000, perm2);

  BOOST_CHECK(endPoint1 == endPoint2);
  BOOST_CHECK(!(endPoint1 == endPoint3));
  BOOST_CHECK(!(endPoint1 == endPoint4));
  BOOST_CHECK(!(endPoint1 == endPoint5));
  BOOST_CHECK(!(endPoint1 == endPoint6));
}

BOOST_AUTO_TEST_CASE(Test_Copy)
{
  using namespace AsioExpress::MessagePort::Ipc;

  EndPoint endPoint1("MessagePortName", 1000, 8000, boost::interprocess::permissions());
  EndPoint endPoint2(endPoint1);

  BOOST_CHECK(endPoint1 == endPoint2);
}

BOOST_AUTO_TEST_CASE(Test_Get_End_Point)
{
  using namespace AsioExpress::MessagePort::Ipc;

  EndPoint endPoint("MessagePortName", 1000, 8000, boost::interprocess::permissions());

  BOOST_CHECK_EQUAL(endPoint.GetEndPoint(), "MessagePortName");
}

BOOST_AUTO_TEST_CASE(Test_Get_Max_Messages)
{
  using namespace AsioExpress::MessagePort::Ipc;

  EndPoint endPoint("MessagePortName", 1000, 8000, boost::interprocess::permissions());

  BOOST_CHECK_EQUAL(endPoint.GetMaxNumMsg(), 1000);
}

BOOST_AUTO_TEST_CASE(Test_Get_Max_Message_Size)
{
  using namespace AsioExpress::MessagePort::Ipc;

  EndPoint endPoint("MessagePortName", 1000, 8000, boost::interprocess::permissions());

  BOOST_CHECK_EQUAL(endPoint.GetMaxMsgSize(), 8000);
}

BOOST_AUTO_TEST_CASE(Test_Get_Permissions)
{
  using namespace AsioExpress::MessagePort::Ipc;

  boost::interprocess::permissions perm1(0762);

  EndPoint endPoint("MessagePortName", 1000, 8000, perm1);

  BOOST_CHECK(endPoint.GetPermissions().get_permissions() == perm1.get_permissions());
}

BOOST_AUTO_TEST_SUITE_END()
