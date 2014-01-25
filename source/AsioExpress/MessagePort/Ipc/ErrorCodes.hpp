//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once 

#include "boost/system/error_code.hpp"

namespace AsioExpress {
namespace MessagePort {
    namespace Ipc {

extern boost::system::error_category const & GetErrorCategory();

namespace ErrorCode
{
  enum Enum
  {
    CommunicationFailure = 1,
    MessageQueueFull,
    BadUsage,
    Disconnected,
    TimeOutExpired,
    MessageQueueSendFailed,
    LostConnection,
 };

  // implicit conversion helper function
  inline boost::system::error_code make_error_code(Enum e)
  {
    return boost::system::error_code(
      static_cast<int>(e), 
      AsioExpress::MessagePort::Ipc::GetErrorCategory());
  }
  // implicit conversion helper function
  inline boost::system::error_condition make_error_condition(Enum e)
  {
    return boost::system::error_condition(
      static_cast<int>(e), 
      AsioExpress::MessagePort::Ipc::GetErrorCategory());
  }
} // namespace ErrorCode

} // namespace Ipc
} // namespace MessagePort
} // namespace AsioExpress

namespace boost {
  namespace system {

template<> struct 
is_error_code_enum<AsioExpress::MessagePort::Ipc::ErrorCode::Enum>
{
  static const bool value = true;
};

} // namespace system
} // namespace boost
