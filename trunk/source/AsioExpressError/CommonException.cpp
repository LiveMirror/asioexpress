//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/CommonException.hpp"

AsioExpress::CommonException::CommonException(Error error) :
  error(error)
{
}

AsioExpress::Error 
AsioExpress::CommonException::GetError() const
{ 
    return error; 
}

char const * 
AsioExpress::CommonException::what() const
{ 
  return error.Message();
}

AsioExpress::CommonException & AsioExpress::CommonException::operator=(AsioExpress::CommonException const &commonException)
{
  error = commonException.error;
  return *this;
}