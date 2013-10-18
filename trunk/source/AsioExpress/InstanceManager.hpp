//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <map>

namespace AsioExpress {

class InstanceManager
{
public:
  unsigned int Next(std::string const & name);

  static InstanceManager* Instance();

  static void CleanUp();

private:
  typedef std::map<std::string, unsigned int> SerialNumberMap;

  SerialNumberMap             m_serialNumber;
  static InstanceManager *    m_instance;
};

}// namespace AsioExpress
