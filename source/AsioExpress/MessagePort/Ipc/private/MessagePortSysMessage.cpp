//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <stdint.h>

#include "AsioExpressConfig/config.hpp"

#include "AsioExpress/MessagePort/Ipc/private/MessagePortSysMessage.hpp"

namespace AsioExpress {
namespace MessagePort {
namespace Ipc {


const char* MessagePortSysMessage::MSG_CONNECT      = "CONNECT";
const char* MessagePortSysMessage::MSG_CONNECT_ACK  = "CONN-ACK";
const char* MessagePortSysMessage::MSG_DISCONNECT   = "DISCONN";


const std::string& MessagePortSysMessage::GetParam(int idx) const
{
  if ( idx < 0 || idx >= GetNumParams() )
    return m_dummy;

  return m_params[idx];
}


bool MessagePortSysMessage::Decode(void* data)
{
  ClearParams();
  std::string tmp;
  char* ptr = (char*) data;

  char buf[9];
  memcpy(buf, ptr, 8);
  buf[8] = '\0';
  m_messageType = buf;
  ptr += 8;

  uint32_t numParams = *(uint32_t*) ptr;
  ptr += sizeof(uint32_t);

  for ( uint32_t i = 0; i < numParams; i++ )
  {
    uint32_t paramSize = *(uint32_t*) ptr;
    ptr += sizeof(uint32_t);

    tmp.assign(ptr, paramSize);
    ptr += paramSize;

    AddParam(tmp);
  }

  return true;
}


int MessagePortSysMessage::Encode(void* buffer)
{
  char* ptr = (char*) buffer;

  if ( GetMessageType().size() < 8 )
  {
    memset(ptr, 0, 8);
    memcpy(ptr, GetMessageType().c_str(), GetMessageType().size());
  }
  else
    memcpy(ptr, GetMessageType().c_str(), 8);

  ptr += 8;

  *(uint32_t*) ptr = (uint32_t) GetNumParams();
  ptr += sizeof(uint32_t);

  for ( int i = 0; i < GetNumParams(); i++ )
  {
    *(uint32_t*) ptr = (uint32_t) GetParam(i).size();
    ptr += sizeof(uint32_t);

    memcpy(ptr, GetParam(i).c_str(), GetParam(i).size());
    ptr += GetParam(i).size();
  }

  return (ptr - (char*) buffer);
}


int MessagePortSysMessage::RequiredEncodeBufferSize()
{
  int size = 8 + sizeof(uint32_t);

  for ( int i = 0; i < GetNumParams(); i++ )
  {
    size += sizeof(uint32_t);
    size += GetParam(i).size();
  }

  return size;
}


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
