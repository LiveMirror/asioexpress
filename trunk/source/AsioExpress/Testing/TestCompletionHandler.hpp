//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Error.hpp"

namespace AsioExpress {
namespace Testing {
  
class TestCompletionHandler
{
public:
  TestCompletionHandler() : 
    m_callCount(new int(0)),
    m_lastError(new AsioExpress::Error)
  {
  }

  void operator()(AsioExpress::Error error)
  {
    ++(*m_callCount);
    *m_lastError = error;
  }

  int Calls()
  {
    return *m_callCount;
  }

  AsioExpress::Error LastError()
  {
    return *m_lastError;
  }

  void Reset()
  {
    *m_callCount = 0;
    *m_lastError = AsioExpress::Error();
  }

private:
  boost::shared_ptr<int>                    m_callCount;
  boost::shared_ptr<AsioExpress::Error>    m_lastError;
};

} // namespace Test
} // namespace AsioExpress
