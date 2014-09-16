//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "boost/system/error_code.hpp"
#include "AsioExpress/Error.hpp"

namespace AsioExpress {

template<typename H>
class EcToErrorAdapter
{
public:
  EcToErrorAdapter(H completionHandler) :
    m_completionHandler(completionHandler)
  {
  }

  void operator()(boost::system::error_code ec = boost::system::error_code())
  {
    m_completionHandler(Error(ec));
  }

private:
  H m_completionHandler;
};

} // namespace AsioExpress
