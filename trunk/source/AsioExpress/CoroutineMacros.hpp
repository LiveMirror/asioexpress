//
// Coroutine.h
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#define CORO_REENTER(c) \
  __pragma(warning(push)) \
  __pragma(warning(disable: 4068)) \
  __pragma(CoverageScanner(cov-off)) \
  switch (AsioExpress::coroutine_ref _coro_value = c) \
    case -1: if (_coro_value) \
    { \
      goto terminate_coroutine; \
      terminate_coroutine: \
      _coro_value = -1; \
      goto bail_out_of_coroutine; \
      bail_out_of_coroutine: \
      break; \
    } \
    __pragma(CoverageScanner(pop)) \
    __pragma(warning(pop)) \
    else case 0:

#define CORO_YIELD \
  __pragma(warning(push)) \
  __pragma(warning(disable: 4068)) \
  __pragma(CoverageScanner(cov-off)) \
  for (_coro_value = __LINE__;;) \
    if (_coro_value == 0) \
    { \
      case __LINE__: ; \
      break; \
    } \
    else \
      switch (_coro_value ? 0 : 1) \
        for (;;) \
          case -1: if (_coro_value) \
            goto terminate_coroutine; \
          else for (;;) \
            case 1: if (_coro_value) \
              goto bail_out_of_coroutine; \
  __pragma(CoverageScanner(pop)) \
  __pragma(warning(pop)) \
            else case 0:

#define CORO_FORK \
  __pragma(warning(push)) \
  __pragma(warning(disable: 4068)) \
  __pragma(CoverageScanner(cov-off)) \
  for (_coro_value = -__LINE__;; _coro_value = __LINE__) \
    if (_coro_value == __LINE__) \
    { \
      case -__LINE__: ; \
      break; \
    } \
  __pragma(CoverageScanner(pop)) \
  __pragma(warning(pop)) \
    else
