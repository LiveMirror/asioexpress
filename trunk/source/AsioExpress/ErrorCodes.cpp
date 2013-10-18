//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpress/ErrorCodes.hpp"

namespace AsioExpress {

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
    case ErrorCode::EventQueueTimeout:
      return "Timed out waiting to receive a queued event.";   
    case ErrorCode::UniqueEventTimeout:
      return "Timed out waiting to receive a unique event.";   
    case ErrorCode::ResourceCacheTimeout:
      return "Timed out waiting to receive an event from the resource cache.";
    case ErrorCode::MessagePortAcceptorError:
      return "Error returned by the message port acceptor.";
  }

  return "Unknown Error";
}

boost::system::error_category const & GetErrorCategory()
{
  static ErrorCategoryImpl instance;
  return instance;
}

} // namespace AsioExpress
