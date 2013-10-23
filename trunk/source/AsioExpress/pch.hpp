//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressConfig/config.hpp"

#ifdef _MSC_VER
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
    #include <winsock2.h>
    #include <mstcpip.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif // _MSC_VER

WIN_DISABLE_WARNINGS_BEGIN(4996)
#include <map>
#include <ostream>
#include <stdexcept> 
#include <string>
WIN_DISABLE_WARNINGS_END

WIN_DISABLE_WARNINGS_BEGIN(4503 4127 4245)
#include <boost/asio.hpp> 
#include <boost/bind.hpp> 
#include <boost/crc.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_ptr.hpp> 
#include <boost/system/error_code.hpp>
#include <boost/system/error_code.hpp> 
#include <boost/system/system_error.hpp> 
#include <boost/thread.hpp> 
#include <boost/thread/condition.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
WIN_DISABLE_WARNINGS_END

#include "AsioExpressError/Check.hpp"
#include "AsioExpressError/EcToErrorAdapter.hpp"

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Error.hpp"
#include "AsioExpress/SimpleStateMachine.hpp"
#include "AsioExpress/MessagePort/DataBuffer.hpp"
#include "AsioExpress/Service/CommandProcessor.hpp"
#include "AsioExpress/Service/CommandProcessorService.hpp"
#include "AsioExpress/Proc/RunProc.hpp"
