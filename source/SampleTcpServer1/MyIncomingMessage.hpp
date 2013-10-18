//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class MyIncomingMessage
{
public:
  bool ParseFromArray(void *buffer, int size)
  {
    m_buffer = buffer;
    m_size = size;
    return true;
  }

  int message_type()
  {
    // Must return a message type here that corresponds to message type 
    // used for the AddMessageHandler call.
    return 1;
  }

  int ByteSize()
  {
    return m_size;
  }

  bool SerializeToArray(void* data, int size)
  {
    memcpy(data, m_buffer, size);
    return true;
  }

private:
  void *  m_buffer;
  int     m_size;
};
