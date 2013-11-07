//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once 

#include "boost/system/error_code.hpp"

namespace AsioExpress {
namespace Common {

extern boost::system::error_category const & GetErrorCategory();

namespace ErrorCode
{
  enum Enum
  {
    BasicException = 1,
    StandardException,
    UnknownException,
    ContractViolation,
    Win32Exception,
    SecureRunTimeException
  };

  // implicit conversion helper function
  inline boost::system::error_code make_error_code(Enum e)
  {
    return boost::system::error_code(
      static_cast<int>(e), 
      AsioExpress::Common::GetErrorCategory());
  }
  // implicit conversion helper function
  inline boost::system::error_condition make_error_condition(Enum e)
  {
    return boost::system::error_condition(
      static_cast<int>(e), 
      AsioExpress::Common::GetErrorCategory());
  }
} // namespace ErrorCode

} // namespace Common
} // namespace AsioExpress

namespace boost {
  namespace system {

template<> struct is_error_code_enum<AsioExpress::Common::ErrorCode::Enum>
{
  static const bool value = true;
};

} // namespace system
} // namespace boost
