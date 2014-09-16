//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressError/Check.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Error.hpp"

namespace AsioExpress {
namespace Proc {

///
/// This class is used to call the completion handler for an async processsor.
///
class Status
{
public:
  Status(
      AsioExpress::CompletionHandler completionHandler) :
    m_completionHandler(completionHandler),
    m_completionHandlerCalls(0)
  {
  }

  bool IsComplete()
  {
    return m_completionHandlerCalls > 0;
  }

  void Complete(AsioExpress::Error error)
  {
    ++m_completionHandlerCalls;
    CHECK_MSG(m_completionHandlerCalls == 1, "The completion handler was called multiple times.");
    m_completionHandler(error);
    m_completionHandler = 0;
  }

private:
  AsioExpress::CompletionHandler   m_completionHandler;
  int                               m_completionHandlerCalls;
};

} // namespace Proc
} // namespace AsioExpress
