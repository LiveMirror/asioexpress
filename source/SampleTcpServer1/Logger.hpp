//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "AsioExpress/Logging/Macros.hpp"
#include "AsioExpress/ClientServer/ServerEventHandler.hpp"

/// 
/// The logger needs to implement the following policy methods.
///
class Logger 
{
public:
  Logger(std::string name, int level=3) :
      m_name(name),
      m_level(level)
  {
  }

  bool isLevelTrace() { return m_level >=6; }
  bool isLevelDebug() { return m_level >= 5; }
  bool isLevelInfo() { return m_level >= 4; }
  bool isLevelWarn() { return m_level >= 3; }
  bool isLevelError() { return m_level >= 2; }
  bool isLevelFatal() { return m_level >= 1; }

  void WriteTrace(std::string message, char const *file, int line)
  {
    std::cout << "+TRACE: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

  void WriteDebug(std::string message, char const *file, int line)
  {
    std::cout << "+DEBUG: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

  void WriteInfo(std::string message, char const *file, int line)
  {
    std::cout << "+INFO: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

  void WriteWarn(std::string message, char const *file, int line)
  {
    std::cout << "+WARN: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

  void WriteError(std::string message, char const *file, int line)
  {
    std::cout << "+ERROR: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

  void WriteFatal(std::string message, char const *file, int line)
  {
    std::cout << "+FATAL: " << message << "  " << m_name << " (" << file << ":" << line << ")\n";
  }

private:
  std::string m_name;
  int         m_level;
};
