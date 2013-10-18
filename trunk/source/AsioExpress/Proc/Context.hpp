//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/utility.hpp>
#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/Proc/Status.hpp"
#include "AsioExpress/Proc/Debugging.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"

namespace AsioExpress {
namespace Proc {

class Context
{
public:
  Context(
      boost::asio::io_service & ioService,
      char const * const processorName,
      AsioExpress::CompletionHandler completionHandler) :
    m_ioService(ioService),
    m_debugging(processorName),
    m_status(completionHandler)
  {
  }

  AsioExpress::Coroutine & Coroutine()
  {
    return m_coroutine;
  }

  boost::asio::io_service & GetIoService()
  {
    return m_ioService;
  }

  void Start()
  {
    m_debugging.ProcStart(m_coroutine);
  }

  void Exit()
  {
    m_debugging.ProcExit(m_coroutine);
  }

  void Complete(AsioExpress::Error error = AsioExpress::Error())
  {
    m_debugging.ProcComplete();
    m_status.Complete(error);
  }

  bool IsComplete()
  {
    return m_status.IsComplete();
  }

private:
  Context & operator=(Context const &);

  boost::asio::io_service &        m_ioService;
  AsioExpress::Proc::Debugging     m_debugging;
  AsioExpress::Coroutine           m_coroutine;
  AsioExpress::Proc::Status        m_status;
};

} // namespace Proc
} // namespace AsioExpress
