//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstring>
#include <string>

#include <boost/shared_ptr.hpp>

namespace AsioExpress {
namespace MessagePort {

class DataBuffer
{
public:
  typedef size_t SizeType;

  DataBuffer() :
    m_size(0),
    m_data(0)
  {
  }

  explicit DataBuffer(SizeType size) :
    m_size(size),
    m_data(new char[size])
  {
  }

  DataBuffer(std::string const & str) :
    m_size(0),
    m_data(0)
  {
    Assign(str.c_str(), str.size());
  }

  DataBuffer(DataBuffer const & b) :
    m_size(0),
    m_data(0)
  {
      Assign(b.m_data, b.m_size);
  }

  ~DataBuffer()
  {
    delete [] m_data;
  }

  DataBuffer & operator=(DataBuffer const & b)
  {
      if (this != &b)
        Assign(b.m_data, b.m_size);

      return *this;
  }

  char *Get() const
  {
    return m_data;
  }

  SizeType Size() const
  {
    return m_size;
  }

  void Resize(SizeType newSize)
  {
    if (newSize == m_size)
        return;
    delete [] m_data;
    m_size = newSize;
    m_data = new char [newSize];
  }

  void Assign(char const *newData, SizeType newSize)
  {
      Resize(newSize);
      memcpy(m_data, newData, newSize);
  }

  bool operator==(DataBuffer const &other) const
  {
    return (m_size == other.m_size) && memcmp(m_data, other.m_data, m_size)==0;
  }

private:

  SizeType   m_size;
  char *     m_data;
};

typedef boost::shared_ptr<DataBuffer> DataBufferPointer;

} // namespace MessagePort
} // namespace AsioExpress
