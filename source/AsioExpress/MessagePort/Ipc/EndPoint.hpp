//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <boost/interprocess/permissions.hpp>

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class EndPoint
{
public:
  EndPoint(
      std::string messagePortName,
      std::size_t maxNumMsg = 100,
      std::size_t maxMsgSize = 1024,
      boost::interprocess::permissions permissions = boost::interprocess::permissions()) :
    m_messagePortName(messagePortName),
    m_maxNumMsg(maxNumMsg),
    m_maxMsgSize(maxMsgSize),
    m_permissions(permissions)
  {
  }

  EndPoint(
      std::string messagePortName,
      boost::interprocess::permissions permissions) :
    m_messagePortName(messagePortName),
    m_permissions(permissions)
  {
  }

  EndPoint(const EndPoint& ep)  :
    m_messagePortName(ep.m_messagePortName),
    m_maxNumMsg(ep.m_maxNumMsg),
    m_maxMsgSize(ep.m_maxMsgSize),
    m_permissions(ep.m_permissions)
  {
  }

  bool operator==(EndPoint const & that) const
  {
    return
      this->m_messagePortName == that.m_messagePortName &&
      this->m_maxNumMsg == that.m_maxNumMsg &&
      this->m_maxMsgSize == that.m_maxMsgSize &&
      this->m_permissions.get_permissions() == that.m_permissions.get_permissions();
  }

  inline const std::string& GetEndPoint() const
  {
    return m_messagePortName;
  }

  inline std::size_t GetMaxNumMsg() const
  {
    return m_maxNumMsg;
  }

  inline const std::size_t GetMaxMsgSize() const
  {
    return m_maxMsgSize;
  }

  inline boost::interprocess::permissions GetPermissions() const
  {
    return m_permissions;
  }

private:
  std::string                       m_messagePortName;
  std::size_t                       m_maxNumMsg;
  std::size_t                       m_maxMsgSize;
  boost::interprocess::permissions  m_permissions;
};

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
