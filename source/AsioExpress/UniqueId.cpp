//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpress/pch.hpp"

#include <sstream>

#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/InstanceManager.hpp"

using namespace std;

namespace AsioExpress {

#ifdef _DEBUG

UniqueId::UniqueId(std::string name)
{
  ostringstream id;

  InstanceManager* instanceManager = InstanceManager::Instance();

  id << name << "-" << instanceManager->Next(name);

  m_fullName = id.str();
}

UniqueId::operator std::string() const
{
  return m_fullName;
}

std::string UniqueId::Get() const
{
  return m_fullName;
}

void UniqueId::CleanUp()
{
  InstanceManager::CleanUp();
}

#else

UniqueId::UniqueId(std::string name)
{
}

#endif
}// namespace AsioExpress
