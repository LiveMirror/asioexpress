//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#include "AsioExpress/DebugTimer/DebugTimers.hpp"

#ifdef _DEBUG

namespace AsioExpress {

class ScopedDebugTimer
{
public:
  ScopedDebugTimer(std::string name, char const * file, int line) :
    m_name(name),
    m_file(file),
    m_line(line)
  {
  }

  ~ScopedDebugTimer()
  {
    FunctionExitDebugTimer(m_name, m_file, m_line);
  }

private:
  ScopedDebugTimer & operator=(ScopedDebugTimer const &);

  std::string const m_name;
  char const * const m_file;
  int const m_line;
};

} // namespace AsioExpress

#endif // _DEBUG
