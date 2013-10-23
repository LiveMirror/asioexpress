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
#define LOG_TRACE(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Trace)
#define LOG_TRACE_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Trace)
#define LOG_TRACE_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Trace)
#define LOG_TRACE_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Trace)
#else
#define LOG_TRACE(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_TRACE_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_TRACE_DIRECT(logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_TRACE_STR_DIRECT(logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_DEBUG)
#define LOG_DEBUG(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Debug)
#define LOG_DEBUG_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Debug)
#define LOG_DEBUG_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Debug)
#define LOG_DEBUG_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Debug)
#else
#define LOG_DEBUG(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_DEBUG_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_DEBUG_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_DEBUG_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_INFO)
#define LOG_INFO(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Info)
#define LOG_INFO_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Info)
#define LOG_INFO_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Info)
#define LOG_INFO_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Info)
#else
#define LOG_INFO(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_INFO_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_INFO_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_INFO_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_WARN)
#define LOG_WARN(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Warn)
#define LOG_WARN_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Warn)
#define LOG_WARN_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Warn)
#define LOG_WARN_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Warn)
#else
#define LOG_WARN(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_WARN_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_WARN_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_WARN_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_ERROR)
#define LOG_ERROR(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Error)
#define LOG_ERROR_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Error)
#define LOG_ERROR_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Error)
#define LOG_ERROR_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Error)
#else
#define LOG_ERROR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_ERROR_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_ERROR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_ERROR_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif

#if !defined(LOG_DISABLE_FATAL)
#define LOG_FATAL(ioService, logger, msg) LOG_MACRO_WRAPPER(ioService, logger, msg, Fatal)
#define LOG_FATAL_STR(ioService, logger, msg) LOG_MACRO_STR_WRAPPER(ioService, logger, msg, Fatal)
#define LOG_FATAL_DIRECT(logger, msg) LOG_MACRO_DIRECT_WRAPPER(logger, msg, Fatal)
#define LOG_FATAL_STR_DIRECT(logger, msg) LOG_MACRO_STR_DIRECT_WRAPPER(logger, msg, Fatal)
#else
#define LOG_FATAL(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_FATAL_STR(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_FATAL_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#define LOG_FATAL_STR_DIRECT(ioService, logger, msg) LOG_MACRO_DO_NOTHING
#endif
