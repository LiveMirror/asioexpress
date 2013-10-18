//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include "AsioExpress/private/InstanceManager.hpp"

namespace AsioExpress {

InstanceManager * InstanceManager::m_instance;

unsigned int InstanceManager::Next(std::string const & name)
{
  return ++m_serialNumber[name];
}

InstanceManager* InstanceManager::Instance()
{
  if (m_instance == NULL)
    m_instance = new InstanceManager;

  return m_instance;
}

void InstanceManager::CleanUp()
{
  delete m_instance;
  m_instance = NULL;
}

}// namespace AsioExpress
