//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressError/pch.hpp"

#include "AsioExpressError/Windows/WinErrorUtility.hpp"

inline void trimTrailingCharacters(std::string & text, std::string const & endChars)
{
  std::string::size_type endpos = text.find_last_not_of(endChars);
  if( endpos != std::string::npos )
  {
    text = text.substr( 0, endpos + 1 );
  }
}

std::string AsioExpress::GetWindowsErrorString(DWORD systemErrorCode)
{
  // get system error
  LPVOID lpMsgBuf = 0;

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    systemErrorCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,
    0, 
    NULL);   

  std::string error(static_cast<const char*>(lpMsgBuf));
  LocalFree(lpMsgBuf);

  trimTrailingCharacters(error, "\r\n");

  return error;
}
