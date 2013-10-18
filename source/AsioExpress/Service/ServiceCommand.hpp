//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "boost/shared_ptr.hpp"
#include "AsioExpress/Error.hpp"

namespace AsioExpress {

class ServiceCommand
{ 
public:
  virtual ~ServiceCommand() {};
  virtual AsioExpress::Error Run() = 0;
};

typedef boost::shared_ptr<ServiceCommand> ServiceCommandPointer;

} // namespace AsioExpress
