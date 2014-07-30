//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class EndPoint
{
public:
  EndPoint(std::string messagePortName, std::size_t maxNumMsg = 100, std::size_t maxMsgSize = 1024) :
    m_messagePortName(messagePortName),
    m_maxNumMsg(maxNumMsg),
    m_maxMsgSize(maxMsgSize)
  {
  }

  EndPoint(const EndPoint& ep)  :
    m_messagePortName(ep.m_messagePortName),
    m_maxNumMsg(ep.m_maxNumMsg),
    m_maxMsgSize(ep.m_maxMsgSize)
  {
  }

  bool operator==(EndPoint const & that) const
  {
      return
          this->m_messagePortName == that.m_messagePortName &&
          this->m_maxNumMsg == that.m_maxNumMsg &&
          this->m_maxMsgSize == that.m_maxMsgSize;
  }

  inline const std::string& GetEndPoint() const       { return m_messagePortName; }
  inline std::size_t GetMaxNumMsg() const             { return m_maxNumMsg; }
  inline const std::size_t GetMaxMsgSize() const      { return m_maxMsgSize; }

private:
  std::string   m_messagePortName;
  std::size_t   m_maxNumMsg;
  std::size_t   m_maxMsgSize;
};

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
