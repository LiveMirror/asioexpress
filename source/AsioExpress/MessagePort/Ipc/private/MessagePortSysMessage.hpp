//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp>

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {

class MessagePortSysMessage
{
public:
  static const char* MSG_CONNECT;
  static const char* MSG_CONNECT_ACK;
  static const char* MSG_DISCONNECT;

  static const int SYS_MSG_PRIORITY = 10;

public:
  inline MessagePortSysMessage(const std::string& messageType = "")
    : m_messageType(messageType)
  { }

  inline const std::string& GetMessageType() const      { return m_messageType; }
  inline int GetNumParams() const                       { return (int) m_params.size(); }
  const std::string& GetParam(int idx) const;

  inline void SetMessageType(const std::string& msg)    { m_messageType = msg; }
  inline void ClearParams()                             { m_params.clear(); }
  inline void AddParam(const std::string& param)        { m_params.push_back(param); }

  bool Decode(void* data);
  int Encode(void* buffer);
  int RequiredEncodeBufferSize();

private:
  std::string               m_messageType;
  std::vector<std::string>  m_params;
  std::string               m_dummy;
};


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
