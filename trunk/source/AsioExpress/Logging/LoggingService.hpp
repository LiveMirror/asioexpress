//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Service/CommandProcessorService.hpp"
#include "AsioExpress/Service/CommandProcessor.hpp"
#include "AsioExpress/NullCompletionHandler.hpp"

namespace AsioExpress {

template<typename L>
class LogTraceCommand : public AsioExpress::ServiceCommand
{
public:
  LogTraceCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteTrace(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogTraceCommand & operator=(LogTraceCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

template<typename L>
class LogDebugCommand : public AsioExpress::ServiceCommand
{
public:
  LogDebugCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteDebug(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogDebugCommand & operator=(LogDebugCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

template<typename L>
class LogInfoCommand : public AsioExpress::ServiceCommand
{
public:
  LogInfoCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteInfo(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogInfoCommand & operator=(LogInfoCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

template<typename L>
class LogWarnCommand : public AsioExpress::ServiceCommand
{
public:
  LogWarnCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteWarn(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogWarnCommand & operator=(LogWarnCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

template<typename L>
class LogErrorCommand : public AsioExpress::ServiceCommand
{
public:
  LogErrorCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteError(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogErrorCommand & operator=(LogErrorCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

template<typename L>
class LogFatalCommand : public AsioExpress::ServiceCommand
{
public:
  LogFatalCommand(
      L logger,
      std::string message, 
      char const *file,
      int line) :
    m_logger(logger),
    m_message(message), 
    m_file(file),
    m_line(line)
  {
  }

  virtual AsioExpress::Error Run()
  {
    m_logger.WriteFatal(m_message, m_file, m_line);  
    return AsioExpress::Error();
  }

private:
  LogFatalCommand & operator=(LogFatalCommand &);

  L                     m_logger;
  std::string           m_message;
  char const *          m_file;
  int                   m_line;
};

class LoggingService : public AsioExpress::CommandProcessorService
{ 
public: 
  explicit LoggingService(
      boost::asio::io_service &ioService, 
      int numberOfThreads=1) : 
    CommandProcessorService(ioService, numberOfThreads)
  { 
  }

  static boost::asio::io_service::id id; 

private:
  void shutdown_service() 
  { 
  } 
}; 

class Logger : public AsioExpress::CommandProcessor<LoggingService>
{
public:
  explicit Logger(
      boost::asio::io_service & ioService) :
    CommandProcessor<LoggingService>(ioService)
  {
  }

  template<typename L>
  void WriteTrace(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogTraceCommand<L>(logger, message, file, line));
  }

  template<typename L>
  void WriteDebug(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogDebugCommand<L>(logger, message, file, line));
  }

  template<typename L>
  void WriteInfo(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogInfoCommand<L>(logger, message, file, line));
  }

  template<typename L>
  void WriteWarn(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogWarnCommand<L>(logger, message, file, line));
  }

  template<typename L>
  void WriteError(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogErrorCommand<L>(logger, message, file, line));
  }

  template<typename L>
  void WriteFatal(
      L &logger,
      std::string message, 
      char const *file,
      int line)
  {
    AsyncExecute(
      new LogFatalCommand<L>(logger, message, file, line));
  }
};

} // namespace AsioExpress
