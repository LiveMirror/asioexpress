//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/CommonErrorCodes.hpp"

namespace AsioExpress {
namespace Common {


class ErrorCategoryImpl : public boost::system::error_category
{
public:
  virtual char const * name() const;
  virtual std::string message(int ev) const;
};

char const * ErrorCategoryImpl::name() const
{
  return "AsioExpressError";
}

std::string ErrorCategoryImpl::message(int ev) const
{
  switch (ev)
  {
    case ErrorCode::BasicException:
      return "Exception";

    case ErrorCode::ContractViolation:
      return "Software Contract Violation";

    case ErrorCode::StandardException:
      return "Caught a standard C++ exception";

    case ErrorCode::UnknownException:
      return "Caught a non-standard exception";

    case ErrorCode::Win32Exception:
      return "Win32 Exception";

    case ErrorCode::SecureRunTimeException:
      return "Secure Run Time Exception";
  }

  return "Unknown Error";
}

boost::system::error_category const & GetErrorCategory()
{
  static ErrorCategoryImpl instance;
  return instance;
}

} //namespace Common
} // namespace AsioExpress
