//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/asio.hpp> 
#include <boost/function.hpp>

#include "AsioExpress/Service/ServiceCommand.hpp"
#include "AsioExpress/CompletionHandler.hpp"

namespace AsioExpress {

template<typename Service>
class CommandProcessor
{ 
public:
  typedef Service service_type;

  explicit CommandProcessor(boost::asio::io_service &ioService) :
    m_service(boost::asio::use_service<Service>(ioService))
  {
  }

  /// Get the ioService associated with the object.
  boost::asio::io_service& get_io_service()
  {
    return m_service.get_io_service();
  }

  template <typename Handler> 
  void AsyncExecute(ServiceCommand * command, Handler handler) 
  { 
      m_service.AsyncExecute(command, handler); 
  } 

  template <typename Handler> 
  void AsyncExecute(ServiceCommandPointer command, Handler handler) 
  { 
      m_service.AsyncExecute(command, handler); 
  } 

  void AsyncExecute(ServiceCommand * command) 
  { 
      m_service.AsyncExecute(command); 
  } 

  void AsyncExecute(ServiceCommandPointer command) 
  { 
      m_service.AsyncExecute(command); 
  } 

protected:
  CommandProcessor & operator=(CommandProcessor const &);

  ~CommandProcessor()
  {
  }

  service_type &  m_service;
};

} // namespace AsioExpress
