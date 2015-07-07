//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpress/MessagePort/Ipc/ErrorCodes.hpp"

namespace AsioExpress {
namespace MessagePort {
    namespace Ipc {

class ErrorCategoryImpl : public boost::system::error_category
{
public:
  virtual char const * name() const ASIOEXPRESS_NOEXCEPT;
  virtual std::string message(int ev) const;
};

char const * ErrorCategoryImpl::name() const ASIOEXPRESS_NOEXCEPT
{
  return "MessagePort IPC";
}

std::string ErrorCategoryImpl::message(int ev) const
{
  switch (ev)
  {
    case ErrorCode::CommunicationFailure:
      return "Communication Failure";

    case ErrorCode::MessageQueueFull:
      return "Message Queue Full";

    case ErrorCode::BadUsage:
      return "Bad Usage";

    case ErrorCode::Disconnected:
      return "Disconnected";

    case ErrorCode::TimeOutExpired:
      return "Time Out Expired";

    case ErrorCode::MessageQueueSendFailed:
      return "Message queue send call failed.";

    case ErrorCode::LostConnection:
      return "Lost connection with peer.";
  }

  return "Unknown Error";
}

boost::system::error_category const & GetErrorCategory()
{
  static ErrorCategoryImpl instance;
  return instance;
}

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress
