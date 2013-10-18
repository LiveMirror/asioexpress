//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

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

  DWORD32 numParams = *(DWORD32*) ptr;
  ptr += sizeof(DWORD32);

  for ( DWORD32 i = 0; i < numParams; i++ )
  {
    DWORD32 paramSize = *(DWORD32*) ptr;
    ptr += sizeof(DWORD32);

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

  *(DWORD32*) ptr = (DWORD32) GetNumParams();
  ptr += sizeof(DWORD32);

  for ( int i = 0; i < GetNumParams(); i++ )
  {
    *(DWORD32*) ptr = (DWORD32) GetParam(i).size();
    ptr += sizeof(DWORD32);

    memcpy(ptr, GetParam(i).c_str(), GetParam(i).size());
    ptr += GetParam(i).size();
  }

  return (ptr - (char*) buffer);
}


int MessagePortSysMessage::RequiredEncodeBufferSize()
{
  int size = 8 + sizeof(DWORD32);

  for ( int i = 0; i < GetNumParams(); i++ )
  {
    size += sizeof(DWORD32);
    size += GetParam(i).size();
  }

  return size;
}


} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
