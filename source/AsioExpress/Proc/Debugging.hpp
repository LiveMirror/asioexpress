//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/utility.hpp>
#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"

namespace AsioExpress {
namespace Proc {

class Debugging
{
public:
  Debugging(
      char const * const processorName) :
    m_processorId(processorName)
  {
  }

  void ProcStart(AsioExpress::Coroutine coroutine)
  {
#ifdef _DEBUG
    AsioExpress::StatementDebugTimer(m_processorId.Get(), m_processorId.Get().c_str(), coroutine.GetCurrentLine());
#else
    (void)coroutine;
#endif
  }

  void ProcExit(AsioExpress::Coroutine coroutine)
  {
#ifdef _DEBUG
    AsioExpress::FunctionExitDebugTimer(m_processorId.Get(), m_processorId.Get().c_str(), coroutine.GetCurrentLine());
#else
    (void)coroutine;
#endif
  }

  void ProcComplete()
  {
#ifdef _DEBUG
    AsioExpress::RemoveStatementDebugTimer(m_processorId, __FILE__, __LINE__);
#endif
  }

private:
  AsioExpress::UniqueId     m_processorId;
};

} // namespace Proc
} // namespace AsioExpress
