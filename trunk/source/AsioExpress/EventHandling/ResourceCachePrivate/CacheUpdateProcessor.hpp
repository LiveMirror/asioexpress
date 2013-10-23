//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/shared_ptr.hpp>

#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"
#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Yield.hpp"

namespace AsioExpress {
namespace ResourceCachePrivate {

template<typename CacheUpdate, typename ItemCount>
class CacheUpdateProcessor : private AsioExpress::Coroutine
{
  public:
    typedef boost::shared_ptr<ItemCount> ItemCountPointer;
    typedef boost::shared_ptr<CacheUpdate> CacheUpdatePointer;
    typedef boost::function<void (CacheUpdatePointer update, CompletionHandler completionHandler)> AsyncUpdateFunction;

    CacheUpdateProcessor(
        ItemCountPointer itemCount,
        AsyncUpdateFunction updateFunction,
        AsioExpress::CompletionHandler completionHandler) : 
      m_processorId("CacheUpdateProcessor"),
      m_update(new CacheUpdate), 
      m_itemCount(itemCount),
      m_updateFunction(updateFunction),
      m_completionHandler(completionHandler)
    {
    }

  void operator()(
      AsioExpress::Error error = AsioExpress::Error())
  {
    try
    {
      if (error)
      {
        OnExit(error);
        return;
      }

      STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, this->GetCurrentLine());

      REENTER(this)
      { 
        YIELD
        {
          m_updateFunction(m_update, *this);
        }

        // clear all cache size information
        m_itemCount->clear();

        // load updated cache size information
        typename CacheUpdate::Iterator  it = m_update->Begin();
        typename CacheUpdate::Iterator end = m_update->End();
        for(; it != end; ++it)
          m_itemCount->insert(typename ItemCount::value_type(it->GetKey(),it->GetSize()));      

        OnExit(error);
      }//REENTER
    }
    ASIOEXPRESS_CATCH_ERROR_AND_DO(OnExit(error))
  }

  private:
    void OnExit(AsioExpress::Error const & error)
    {
      STOP_STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, __LINE__);
      m_completionHandler(error);
    }

    AsioExpress::UniqueId               m_processorId;
    CacheUpdatePointer                  m_update;
    ItemCountPointer                    m_itemCount;
    AsyncUpdateFunction                 m_updateFunction;
    AsioExpress::CompletionHandler      m_completionHandler;
};

} // namespace ResourceCachePrivate
} // namespace AsioExpress
