//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Timer/Timer.hpp"

namespace AsioExpress {
namespace ResourceCachePrivate {

template<typename ResourceItem, typename Key>
struct CacheRequest
{
  typedef boost::shared_ptr<ResourceItem> ResourceItemPointer;
  CacheRequest()
  {
  }
  CacheRequest(
      Key key,
      ResourceItemPointer item, 
      CompletionHandler completionHandler,
      TimerPointer waitTimer) :
    key(key),
    item(item),
    completionHandler(completionHandler),
    timer(waitTimer)
  {
  }
  bool operator==(Key const & key) const
  {
    return this->key == key;
  }
  bool operator==(ResourceItemPointer const & item) const
  {
    return this->item == item;
  }
  Key                 key;
  ResourceItemPointer item;
  CompletionHandler   completionHandler;
  TimerPointer        timer;
};  

} // namespace ResourceCachePrivate
} // namespace AsioExpress
