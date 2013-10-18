//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/test/unit_test.hpp>
#include "AsioExpress/Error.hpp"

namespace AsioExpress {
namespace Testing {
  
class AutoCompletionHandler
{
public:
  AutoCompletionHandler(char const * file, int line) : 
    m_file(file), 
    m_line(line),
    m_callCount(new int(0))
  {
  }

  void operator()(AsioExpress::Error error)
  {
    if (error)
    {
      std::ostringstream message;
      message << m_file << "(" << m_line << "): Completion handler error: " << error.Message(); 
      BOOST_FAIL(message);
    }

    ++(*m_callCount);
  }

  int Calls()
  {
    return *m_callCount;
  }

  void Reset()
  {
    *m_callCount = 0;
  }

private:
  char const *            m_file;
  int const               m_line;
  boost::shared_ptr<int>  m_callCount;
};

} // namespace Test
} // namespace AsioExpress
