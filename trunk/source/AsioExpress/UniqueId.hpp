//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <boost/scoped_ptr.hpp>

namespace AsioExpress {

#ifdef _DEBUG
  class UniqueId
  {
  public:
    UniqueId(std::string name);

    operator std::string() const;

    std::string Get() const;

    static void CleanUp();
  private:
    std::string m_fullName;
  };
#else
  class UniqueId
  {
  public:
    UniqueId(std::string name);
  };
#endif

} // namespace AsioExpress
