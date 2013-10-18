//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTRACT_CHECK_H
#define CONTRACT_CHECK_H

#include <string>

#include "AsioExpressError/ContractViolationException.hpp"
#include "AsioExpressError/private/Contract.hpp"

/// @def CHECK(condition)
/// Checks the indicated boolean condition and throws a 
/// ContractViolationException if it evaluates false. 
/// @parm condition A boolean expression
/// @note This check can be disabled for the release build if the preprocessor
/// symbol NO_CONTRACT_CHECK_FOR_RELEASE_BUILD is defined.
/// @sa ContractViolationException

/// @def CHECK_MSG(condition, msg)
/// Checks the indicated boolean condition and throws a 
/// ContractViolationException if it evaluates false. 
/// @param condition A boolean expression
/// @param msg A message to elaborate on the error condition
/// @note This check can be disabled for the release build if the preprocessor
/// symbol NO_CONTRACT_CHECK_FOR_RELEASE_BUILD is defined.
/// @note Use boost::format to create a printf style formatted message string.
/// @sa ContractViolationException boost::format

/// @def CHECK_FAILED(msg) 
/// Reports an error by throwing a ContractViolationException. 
/// @param msg A message to elaborate on the error condition
/// @note This check can be disabled for the release build if the preprocessor
/// symbol NO_CONTRACT_CHECK_FOR_RELEASE_BUILD is defined.
/// @sa ContractViolationException

#if defined(NO_CONTRACT_CHECK_FOR_RELEASE_BUILD) && defined(NDEBUG)
    #define CHECK(condition) (void)(condition)
    #define CHECK_MSG(condition, msg) (void)(condition)
    #define CHECK_FAILED(msg) (void)(msg)
#else
    #define CHECK(cond) \
       (void)(  ((cond) || (throw AsioExpress::ContractViolationException(AsioExpress::errorReport(#cond, std::string(), __FILE__, __LINE__)),0)) \
             )     

    #define CHECK_MSG(cond, msg) \
       (void)(  ((cond) || (throw AsioExpress::ContractViolationException(AsioExpress::errorReport(#cond, (msg), __FILE__, __LINE__)),0)) \
             )     

    #define CHECK_FAILED(msg) \
       (void)(  throw AsioExpress::ContractViolationException(AsioExpress::errorReport(0, (msg), __FILE__, __LINE__)) \
             )     
#endif

/// Checks the indicated static expression and generates a compiler error if
/// it evaluates false.
/// @param An expression that can be evaluated at compile time. A runtime
/// expression will generate a campiler error.
#define STATIC_CHECK(staticExpression)                                            \
  __pragma(warning(push))                                                         \
  __pragma(warning(disable: 4127))                                                \
  do { typedef char contractStaticCheck[(staticExpression) ? 1 : -1]; } while(0)  \
  __pragma(warning(pop))

#endif //CONTRACT_CHECK_H
