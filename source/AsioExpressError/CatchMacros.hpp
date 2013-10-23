//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressError/CommonErrorCodes.hpp"
#include "AsioExpressError/CommonException.hpp"

#ifdef _MSC_VER
#define ASIOEXPRESS_CATCH_ERROR_AND_DO(statement) \
  __pragma(warning(push)) \
  __pragma(warning(disable: 4068)) \
  __pragma(CoverageScanner(cov-off)) \
  catch (AsioExpress::CommonException const & e) { AsioExpress::Error error(e.GetError()); statement; } \
  catch (std::exception const & e) { AsioExpress::Error error(AsioExpress::CommonErrorCode::StandardException, e.what()); statement; } \
  catch (...) { AsioExpress::Error error(AsioExpress::CommonErrorCode::UnknownException); statement; } \
  __pragma(CoverageScanner(pop)) \
  __pragma(warning(pop))
#else // _MSC_VER
#define ASIOEXPRESS_CATCH_ERROR_AND_DO(statement) \
  catch (AsioExpress::CommonException const & e) { AsioExpress::Error error(e.GetError()); statement; } \
  catch (std::exception const & e) { AsioExpress::Error error(AsioExpress::CommonErrorCode::StandardException, e.what()); statement; } \
  catch (...) { AsioExpress::Error error(AsioExpress::CommonErrorCode::UnknownException); statement; }
#endif // _MSC_VER

#define THROW_ON_ERROR(error) if (error) throw AsioExpress::CommonException(error)
