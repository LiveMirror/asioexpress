//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <map>
#include <algorithm>

#include <boost/shared_ptr.hpp>

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/UniqueId.hpp"
#include "AsioExpress/DebugTimer/DebugTimerMacros.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Coroutine.hpp"
#include "AsioExpress/Yield.hpp"

namespace AsioExpress {
namespace ResourceCachePrivate {

template<typename ResourceItem, typename Key>
class AddResourceItemsProcessor : private AsioExpress::Coroutine
{
  public:
    typedef boost::shared_ptr<ResourceItem> ResourceItemPointer;
    typedef boost::function<void (Key key, ResourceItemPointer item, CompletionHandler completionHandler)> AsyncRemoveFunction;
    typedef ResourceCachePrivate::CacheRequest<ResourceItem, Key> Request;
    typedef boost::shared_ptr<Request> RequestPointer;
    typedef std::vector<Request> Requests;
    typedef boost::shared_ptr<Requests> RequestsPointer;
    typedef std::map<Key,int> ItemCount;
    typedef boost::shared_ptr<ItemCount> ItemCountPointer;

    AddResourceItemsProcessor(
        Key key,
        RequestsPointer requests,
        ItemCountPointer itemCount,
        AsyncRemoveFunction removeFunction,
        CompletionHandler errorHandler) :
      m_processorId("AddResourceItemsProcessor"),
      m_key(key),
      m_request(new Request),
      m_requests(requests),
      m_itemCount(itemCount),
      m_removeFunction(removeFunction),
      m_errorHandler(errorHandler)
    {
    }

  void operator()(
      AsioExpress::Error error = AsioExpress::Error())
  {
    try
    {
      STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, this->GetCurrentLine());

      REENTER(this)
      {
        while ((*m_itemCount)[m_key] > 0)
        {
          // remove request from queue if available
          {
            typename Requests::iterator r = std::find(
              m_requests->begin(),
              m_requests->end(),
              m_key);
            if (r == m_requests->end())
            {
              // no requests waiting.
              break;
            }

            *m_request = *r;
            m_requests->erase(r);
          }

          YIELD
          {
            --(*m_itemCount)[m_key];
            m_removeFunction(m_key, m_request->item, *this);
          }

          // call completion handler
          m_request->completionHandler(error);
          m_request->completionHandler = 0;
        }

        OnExit(error);
      }//REENTER
    }
    ASIOEXPRESS_CATCH_ERROR_AND_DO(OnExit(error))
  }

  private:
    void OnExit(AsioExpress::Error const & error)
    {
      STOP_STATEMENT_DEBUG_TIMER(m_processorId, __FILE__, __LINE__);
      if (error)
        m_errorHandler(error);
    }

    AsioExpress::UniqueId       m_processorId;
    Key                                 m_key;
    RequestPointer                      m_request;
    RequestsPointer                     m_requests;
    ItemCountPointer                    m_itemCount;
    AsyncRemoveFunction                 m_removeFunction;
    CompletionHandler                   m_errorHandler;
};

} // namespace ResourceCachePrivate
} // namespace AsioExpress
