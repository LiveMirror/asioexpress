//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/error.hpp>

namespace AsioExpress {

class Error
{
  friend std::ostream & operator<<(std::ostream & outputStream, Error const & error);
public:
  Error() {};

  Error(boost::system::error_code errorCode);

  Error(boost::system::error_code errorCode, std::string description);

  Error & operator=(Error const & error);

  boost::system::error_code GetErrorCode() const;

  bool IsAsioError() const;

  char const * Message() const;

  operator bool () const;

private:
  boost::system::error_code   m_errorCode;
  std::string                 m_errorMessage;
};

inline std::string PrivateMakeErrorMessage(
    boost::system::error_code errorCode, 
    std::string const & description)
{
  std::string message;

  message += errorCode.category().name();
  message += "; ";
  message += errorCode.message();
  message += ": ";
  message += description;

  return message;
}

inline Error::Error(boost::system::error_code errorCode) :
  m_errorCode(errorCode),
  m_errorMessage(PrivateMakeErrorMessage(errorCode, ""))
{
}

inline Error::Error(boost::system::error_code errorCode, std::string description) :
  m_errorCode(errorCode),
  m_errorMessage(PrivateMakeErrorMessage(errorCode, description))
{
}

inline Error & Error::operator=(Error const & error)
{
  m_errorCode = error.m_errorCode;
  m_errorMessage = error.m_errorMessage;
  return *this;
}

inline boost::system::error_code Error::GetErrorCode() const
{
  return m_errorCode;
}

inline bool Error::IsAsioError() const
{
  return 
    m_errorCode.category() == boost::asio::error::get_system_category() ||
    m_errorCode.category() == boost::asio::error::get_addrinfo_category() ||
    m_errorCode.category() == boost::asio::error::get_misc_category();
}

inline char const * Error::Message() const
{
  return m_errorMessage.c_str();
}

inline Error::operator bool() const
{
  return m_errorCode != 0;
}

inline std::ostream & 
operator<<(
        std::ostream & outputStream, 
        Error const & error)
{
  outputStream << error.Message();
  return outputStream;
}

} // namespace AsioExpress

