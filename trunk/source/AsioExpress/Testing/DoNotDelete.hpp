//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
namespace AsioExpress {
namespace Testing {

///
/// Use this as a customized deleter for a smart pointer. It will not delete
/// the pointer when the the smart pointer releases the object. For use with
/// mocking frameworks.
///
struct DoNotDelete
{
  void operator()(void*){};
};

} // namespace Testing
} // namespace AsioExpress
