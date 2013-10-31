//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define ASIOEXPRESS_LOGGING_MACROS_H_INCLUDED

#include <sstream>
#include "AsioExpress/Logging/LoggingService.hpp"

// Additional symbol to disable all logging.
#ifdef LOG_DISABLE_ALL
#define LOG_DISABLE_TRACE
#define LOG_DISABLE_DEBUG
#define LOG_DISABLE_INFO
#define LOG_DISABLE_WARN
#define LOG_DISABLE_ERROR
#define LOG_DISABLE_FATAL
#endif

#define LOG_MACRO_DO_NOTHING ((void)0)

#define LOG_MACRO_BODY(ioService, logger, msg, logLevel)                \
    do {                                                                \
        if((logger).isLevel##logLevel()) {                              \
            std::ostringstream logStream;                               \
            logStream << msg;                                           \
            AsioExpress::Logger log(ioService);                         \
            log.Write##logLevel(logger, logStream.str(), __FILE__, __LINE__); \
        }                                                               \
    } while (0)

#define LOG_MACRO_STR_BODY(ioService, logger, msg, logLevel)            \
    do {                                                                \
        if((logger).isLevel##logLevel()) {                              \
            AsioExpress::Logger log(ioService);                         \
            log.Write##logLevel(logger, msg, __FILE__, __LINE__);                 \
        }                                                               \
    } while(0)

#define LOG_MACRO_DIRECT_BODY(logger, msg, logLevel)                \
    do {                                                                \
        if((logger).isLevel##logLevel()) {                              \
            std::ostringstream logStream;                               \
            logStream << msg;                                           \
            logger.Write##logLevel(logStream.str(), __FILE__, __LINE__); \
        }                                                               \
    } while (0)

#define LOG_MACRO_STR_DIRECT_BODY(logger, msg, logLevel)            \
    do {                                                                \
        if((logger).isLevel##logLevel()) {                              \
            logger.Write##logLevel(msg, __FILE__, __LINE__);                 \
        }                                                               \
    } while(0)

#ifdef _MSC_VER
    #define LOG_MACRO_DISABLE_WARNINGS \
      __pragma(warning(push))                             \
      __pragma(warning(disable: 4127 4068))                    \
      __pragma(CoverageScanner(cov-off))                    

    #define LOG_MACRO_ENABLE_WARNINGS \
      __pragma(CoverageScanner(pop))                   \
      __pragma(warning(pop))
#else  // _MSC_VER
    #define LOG_MACRO_DISABLE_WARNINGS
    #define LOG_MACRO_ENABLE_WARNINGS
#endif // _MSC_VER

#define LOG_MACRO_WRAPPER(ioService,logger,msg,type) \
    LOG_MACRO_DISABLE_WARNINGS \
    LOG_MACRO_BODY (ioService, logger, msg, type)  \
    LOG_MACRO_ENABLE_WARNINGS
#define LOG_MACRO_STR_WRAPPER(ioService,logger,msg,type) \
    LOG_MACRO_DISABLE_WARNINGS \
    LOG_MACRO_STR_BODY(ioService, logger, msg, type)  \
    LOG_MACRO_ENABLE_WARNINGS
#define LOG_MACRO_DIRECT_WRAPPER(logger,msg,type) \
    LOG_MACRO_DISABLE_WARNINGS \
    LOG_MACRO_DIRECT_BODY (logger, msg, type)  \
    LOG_MACRO_ENABLE_WARNINGS
#define LOG_MACRO_STR_DIRECT_WRAPPER(logger,msg,type) \
    LOG_MACRO_DISABLE_WARNINGS \
    LOG_MACRO_STR_DIRECT_BODY (logger, msg, type)  \
    LOG_MACRO_ENABLE_WARNINGS

#if !defined(LOG_DISABLE_TRACE)
#define TRACE_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Trace)
#define TRACE_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Trace)
#define TRACE_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Trace)
#define TRACE_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Trace)
#else
#define TRACE_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define TRACE_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define TRACE_MSG_DIRECT(logger, msg) LOG_MACRO_DO_NOTHING
#define TRACE_STR_DIRECT(logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_DEBUG)
#define DEBUG_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Debug)
#define DEBUG_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Debug)
#define DEBUG_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Debug)
#define DEBUG_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Debug)
#else
#define DEBUG_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define DEBUG_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define DEBUG_MSG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define DEBUG_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_INFO)
#define INFO_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Info)
#define INFO_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Info)
#define INFO_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Info)
#define INFO_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Info)
#else
#define INFO_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define INFO_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define INFO_MSG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define INFO_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_WARN)
#define WARN_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Warn)
#define WARN_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Warn)
#define WARN_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Warn)
#define WARN_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Warn)
#else
#define WARN_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define WARN_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define WARN_MSG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define WARN_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_ERROR)
#define ERROR_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Error)
#define ERROR_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Error)
#define ERROR_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Error)
#define ERROR_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Error)
#else
#define ERROR_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define ERROR_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define ERROR_MSG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define ERROR_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_FATAL)
#define FATAL_MSG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Fatal)
#define FATAL_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Fatal)
#define FATAL_MSG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Fatal)
#define FATAL_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Fatal)
#else
#define FATAL_MSG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define FATAL_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define FATAL_MSG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define FATAL_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif
