//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpress/MessagePort/Tcp/TcpErrorCodes.hpp"

namespace AsioExpress {
namespace MessagePort {
    namespace Tcp {

class ErrorCategoryImpl : public boost::system::error_category
{
public:
  virtual char const * name() const;
  virtual std::string message(int ev) const;
};

char const * ErrorCategoryImpl::name() const
{
  return "MessagePort TCP";
}

std::string ErrorCategoryImpl::message(int ev) const
{
  switch (ev)
  {
    case ErrorCode::ProtocolError:
      return "Received message is not using the correct protocol or "
             "we have lost syncronization with the sender.";

    case ErrorCode::WrongProtocolVersion:
      return "Received message is not using the correct protocol; "
             "the protocol version different than expected.";

    case ErrorCode::SocketInitializationFailed:
      return "Socket initialization failed.";
  }

  return "Unknown Error";
}

boost::system::error_category const & GetErrorCategory()
{
  static ErrorCategoryImpl instance;
  return instance;
}

} // namespace Tcp
} // namespace MessagePort
} // namespace AsioExpress
