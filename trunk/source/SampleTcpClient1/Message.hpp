//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <stdio.h>

#include "AsioExpress/MessagePort/DataBuffer.hpp"

class Message
{
public:
  Message() {};

  Message(std::string command) :
      m_text(command)
  {
  }

  void ParseFromArray(void *buffer, int size)
  {
    char* tempBuffer = new char [size + 1];

    memcpy(tempBuffer, buffer, size);
    tempBuffer[size] = '\0';

    m_text = tempBuffer;

    delete [] tempBuffer;
  }

  AsioExpress::MessagePort::DataBufferPointer GetDataBuffer()
  {
    AsioExpress::MessagePort::DataBufferPointer data(new AsioExpress::MessagePort::DataBuffer(m_text.length() + 1));
    strcpy(data->Get(), m_text.c_str());
    return data;
  }

  std::string GetText() 
  { 
    return m_text; 
  }

private:
  std::string m_text;
};

typedef boost::shared_ptr<Message> CommandMessagePointer;
