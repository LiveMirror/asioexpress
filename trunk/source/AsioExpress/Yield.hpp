//
// Yield.h
// ~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "AsioExpress/CoroutineMacros.hpp"

#ifndef REENTER
# define REENTER(c) CORO_REENTER(c)
#endif

#ifndef YIELD
# define YIELD CORO_YIELD
#endif

#ifndef FORK
# define FORK CORO_FORK
#endif
