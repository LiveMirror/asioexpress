//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

WIN_DISABLE_WARNINGS_BEGIN(disable: 4244)
#include "boost/shared_ptr.hpp"
WIN_DISABLE_WARNINGS_END

#include "AsioExpressError/Check.hpp"

namespace AsioExpress {

///
/// A specialized type for holding a reference to a smart pointer.
/// It may be used to return a smart pointer as an output parameter or to hold
/// an assignable pointer value in a class that gets copied.
/// It is based upon boost::shared_ptr.
/// 
template <class B, class H=boost::shared_ptr<B> > 
class PointerHandle
{
public:
  typedef B base_pointer_type;
  typedef H handle_type;

  ///
  /// The default constructor automatically initializes the handle with a null 
  /// pointer instance.
  ///
  PointerHandle() :
    handle(new base_pointer_type())
  {
  }

  ///
  /// Use this method to get the value of the handle which is a pointer.
  ///
  base_pointer_type get()
  {
    return *handle;
  }

  ///
  /// Use this method to set the value of the handle with a new pointer.
  ///
  void set(base_pointer_type p)
  {
    *handle = p;
  }

  ///
  /// Equality means it is a handle to the same shared pointer.
  ///
  bool operator==(PointerHandle const & that) const
  {
    return this->handle.get() == that.handle.get();
  }

  ///
  /// Inequality means it is not a handle to the same shared pointer.
  ///
  bool operator!=(PointerHandle const & that) const
  {
    return ! operator==(that);
  }

private:
  handle_type handle;
};

} // namespace AsioExpress
