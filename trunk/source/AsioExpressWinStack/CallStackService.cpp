//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable: 4244 4512)
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#pragma warning(pop)

#include "private/StackWalker.h"
#include "AsioExpressError/CallStack.hpp"
#include "CallStackService.h"

using namespace AsioExpress;
using namespace AsioExpress;

///
/// This class is used to get a stack trace for error reporting purposes.
/// It requires debugging information (.PDB files) to be deployed with your
/// executables.
///
class CallStackService : public StackWalker
{
public:
    ///
    /// Constructor for CallStackService.
    ///
    CallStackService() :
      StackWalker(OptionsAll, NULL, GetCurrentProcessId(), GetCurrentProcess()),
      verbose(false)
    {
    }

    ///
    /// Call this to generate module and version information along with the call stack.
    ///
    void SetVerboseMode()
    {
        verbose = true;
    }

    ///
    /// Call this to generate only call stack information.
    ///
    void SetBriefMode()
    {
        verbose = false;
    }

    ///
    /// This method is returns a message with the call stack details.
    /// @return Text string with call stack inforamtion. 
    ///
    std::string GetDetails()
    {
        return messageStream.str();
    }

    ///
    /// Resets the call stack details.
    ///
    void Reset()
    {
        messageStream.clear();
        messageStream.str("");
    }

protected:
  virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName)
  {
      if (verbose)
          StackWalker::OnSymInit(szSearchPath, symOptions, szUserName);
  }

  virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion)
  {
      if (verbose)
          StackWalker::OnLoadModule(img, mod, baseAddr, size, result, symType, pdbName, fileVersion);
  }

  virtual void OnOutput(LPCSTR szText)
  { 
      messageStream << szText;

      StackWalker::OnOutput(szText); 
  }

private:
    std::ostringstream messageStream;
    bool verbose;
};

namespace
{
  typedef boost::unique_lock<boost::mutex> LockGuard;

  boost::once_flag    onceFlag = BOOST_ONCE_INIT;
  CallStackService *  callStackInformation;
  boost::mutex *      callStackLock;
}

void initCallStack()
{
  // the call once routine must not throw
  try
  {
    callStackInformation = new CallStackService;
    callStackLock = new boost::mutex;
  }
  catch(...)
  {
    delete callStackInformation;
    callStackInformation = 0;
    delete callStackLock;
    callStackLock = 0;
  }
}

void AsioExpress::StartCallStackService()
{
  boost::call_once(&initCallStack, onceFlag);
}

void AsioExpress::StopCallStackService()
{
  delete callStackInformation;
  callStackInformation = 0;

  delete callStackLock;
  callStackLock = 0;
}

std::string AsioExpress::WinCallStackHandler(
  HANDLE thread, 
  const CONTEXT *context)
{
  boost::call_once(&initCallStack, onceFlag);

  if (callStackLock == NULL)
  {
    return "Error encountered initializing the call stack service.";
  }

  LockGuard lock(*callStackLock);

  callStackInformation->ShowCallstack(thread, context);

  std::string details = callStackInformation->GetDetails();

  callStackInformation->Reset();

  return details;
}

std::string AsioExpress::CallStackHandler()
{
  return WinCallStackHandler();
}
