//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Timer/Timer.hpp"
#include "AsioExpress/EventHandling/ResourceCachePrivate/CacheRequest.hpp"
#include "AsioExpress/EventHandling/ResourceCachePrivate/CacheUpdateProcessor.hpp"
#include "AsioExpress/EventHandling/ResourceCachePrivate/AddResourceItemsProcessor.hpp"

namespace AsioExpress {

template<typename R, typename K = int>
class ResourceCache
{
public:
  typedef R ResourceItem;
  typedef K Key;
  typedef boost::shared_ptr<ResourceItem> ResourceItemPointer;

  ResourceCache();

  ///
  /// Determines the currently active caches and their sizes. You need to call 
  /// this to properly initialize the resource cache and perhaps occasionally 
  /// to remove caches that are no longer in use.
  /// @param completionHandler  The completion hander is called when the 
  ///                           operation is complete.
  ///
  void AsyncUpdate(
      CompletionHandler completionHandler);

  ///
  /// Gets a resource item from the resource cache. The particular cache to
  /// use is identified by the key. 
  /// @param key                Identifies the cache to rerieve the item.
  /// @param item               A pointer to a resource item. This item will
  ///                           be set to a valid resource item if the operation
  ///                           successfully completes.
  /// @param waitTimer          How long to wait to receive a resource item.
  /// @param completionHandler  The completion hander is called when the 
  ///                           operation is complete.
  ///
  void AsyncGet(
      Key key,
      ResourceItemPointer item, 
      TimerPointer waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// Gets a resource item from the resource cache. Use this if you do not need
  /// multiple resource caches.
  /// 
  /// @param item               A pointer to a resource item. This item will
  ///                           be set to a valid resource item if the operation
  ///                           successfully completes.
  /// @param waitTimer          How long to wait to receive a resource item.
  /// @param completionHandler  The completion hander is called when the 
  ///                           operation is complete.
  ///
  void AsyncGet(
      ResourceItemPointer item, 
      TimerPointer waitTimer, 
      CompletionHandler completionHandler);

  ///
  /// Notify the resource cache that items have been added.
  /// @param key        Identifies the cache.
  /// @param count      The number of resoure items added to the cache.
  ///
  void Add(Key key, int count);

  ///
  /// Notify the resource cache that items have been added. Use this if you do 
  /// not need multiple resource caches.
  /// @param count      The number of resoure items added to the cache.
  ///
  void Add(int count);

  ///
  /// Gets the size of a resource cache.
  /// @param key        Identifies the cache.
  ///
  unsigned int GetCacheSize(Key key) const;

  ///
  /// This method cancels any pending operations on all caches. If
  /// AsyncGet is called on a canceled cache an operation aborted error 
  /// is returned immediately.
  ///
  void ShutDown();

public:
  class CacheSize
  {
  public:
    CacheSize(Key key, unsigned int size): m_key(key), m_size(size) {}
    Key GetKey() { return m_key; }
    unsigned int GetSize() { return m_size; }
    bool operator==(CacheSize const & that) const { return m_key == that.m_key && m_size == that.m_size; }
  private:
    Key             m_key;
    unsigned int    m_size;
  };

  ///
  /// Class used to update the active resource caches.
  ///
  class CacheUpdate
  {
  public:
    typedef std::vector<CacheSize> CacheSizes;
    typedef typename CacheSizes::iterator Iterator;
    void SetSize(Key key, unsigned int size) { m_sizes.push_back(CacheSize(key,size)); }
    Iterator Begin() { return m_sizes.begin(); }
    Iterator End() {return m_sizes.end(); }
    bool operator==(CacheUpdate const & that) const { return m_sizes == that.m_sizes; }
    bool operator!=(CacheUpdate const & that) const { return !operator==(that); }
  private:
    CacheSizes m_sizes;
  };

  typedef boost::shared_ptr<CacheUpdate> CacheUpdatePointer;

protected:
  ///
  /// This virtual method must be implemented by derived classes. It is used 
  /// to extract the resource item from implemenattion defined storage.
  /// @param key                Identifies the cache to rerieve the item.
  /// @param item               A pointer to a resource item. This item will
  ///                           be set to a valid resource item if the operation
  ///                           successfully completes.
  /// @param completionHandler  The completion hander is called when the 
  ///                           operation is complete.
  ///
  virtual void AsyncRemoveItem(
      Key key,
      ResourceItemPointer item, 
      CompletionHandler completionHandler) = 0;

  ///
  /// This virtual method must be implemented by derived classes. It is used 
  /// to determine the currently active caches and their sizes. You need to call 
  /// this to properly initialize the resource cache and occasionally to remove 
  /// caches that are no longer in use.
  /// Call update->SetSize(key,size) to update cache size of active resource
  /// caches.
  /// @param update             The cache update object used to update the
  ///                           resource cache information.
  /// @param completionHandler  The completion hander is called when the 
  ///                           operation is complete.
  ///
  virtual void AsyncUpdateAllItems(
      CacheUpdatePointer update, 
      CompletionHandler completionHandler) = 0;

  ///
  /// If an error occurs as a result of an Add() operation this virtual method
  /// is called for the derived class to handle.
  ///
  virtual void ErrorHandler(
    AsioExpress::Error error) = 0;

private:
  typedef ResourceCachePrivate::CacheRequest<ResourceItem, Key> Request;
  typedef std::vector<Request> Requests;
  typedef boost::shared_ptr<Requests> RequestsPointer;
  typedef std::map<Key,int> ItemCount;
  typedef boost::shared_ptr<ItemCount> ItemCountPointer;

  void Timeout(AsioExpress::Error error, ResourceItemPointer item);
  void ReturnError(AsioExpress::Error error, ResourceItemPointer item);
  bool CanDelete(Request const & request, Key key);

  RequestsPointer     m_requests;
  ItemCountPointer    m_itemCount;
  bool                m_isShutDown;
};

template<typename R, typename K>
ResourceCache<R,K>::ResourceCache() :
  m_requests(new Requests),
  m_itemCount(new ItemCount),
  m_isShutDown(false)
{
}

template<typename R, typename K>
void ResourceCache<R,K>::AsyncUpdate(
    CompletionHandler completionHandler)
{
  ResourceCachePrivate::CacheUpdateProcessor<CacheUpdate, ItemCount> proc(
    m_itemCount,
    boost::bind(&ResourceCache<R,K>::AsyncUpdateAllItems, this, _1, _2),
    completionHandler);

  proc();
}

template<typename R, typename K>
void ResourceCache<R,K>::AsyncGet(
      Key key,
      ResourceItemPointer item, 
      TimerPointer waitTimer, 
      CompletionHandler completionHandler)
{
  // If the event has been canceled return operation aborted immediately.
  if ( m_isShutDown )
  {
    completionHandler(Error(boost::asio::error::operation_aborted));
    return;
  }

  if ((*m_itemCount)[key] > 0)
  {
    --(*m_itemCount)[key];
    AsyncRemoveItem(key, item, completionHandler);
    return;
  }

  // Queue this event to be handled. 
  //
  waitTimer->AsyncWait(boost::bind(&ResourceCache<R,K>::Timeout, this, _1, item));

  m_requests->push_back(
    Request(key, item, completionHandler, waitTimer));
}

template<typename R, typename K>
void ResourceCache<R,K>::AsyncGet(
      ResourceItemPointer item, 
      TimerPointer waitTimer, 
      CompletionHandler completionHandler)
{
  AsyncGet(Key(), item, waitTimer, completionHandler);
}

template<typename R, typename K>
void ResourceCache<R,K>::Add(Key key, int count)
{
  (*m_itemCount)[key] += count;

  ResourceCachePrivate::AddResourceItemsProcessor<ResourceItem, Key> proc(
    key,
    m_requests,
    m_itemCount,
    boost::bind(&ResourceCache<R,K>::AsyncRemoveItem, this, _1, _2, _3),
    boost::bind(&ResourceCache<R,K>::ErrorHandler, this, _1));

  proc();
}

template<typename R, typename K>
void ResourceCache<R,K>::Add(int count)
{
  Add(Key(), count);
}

//template<typename R, typename K>
//void ResourceCache<R,K>::Delete(Key key)
//{
//  Requests::iterator   begin = m_requests->begin();
//  Requests::iterator     end = m_requests->end();
//  Requests::iterator new_end = std::remove_if(
//    begin,
//    end,
//    boost::bind(&ResourceCache<R,K>::CanDelete, this, _1, key));
//
//  m_requests->erase(new_end, end);
//
//  m_itemCount->erase(key);
//}

template<typename R, typename K>
unsigned int ResourceCache<R,K>::GetCacheSize(Key key) const
{
  return (*m_itemCount)[key];
}

template<typename R, typename K>
void ResourceCache<R,K>::ShutDown()
{
  // Indicate that this event is canceled.
  m_isShutDown = true;

  typename Requests::iterator request = m_requests->begin();
  typename Requests::iterator     end = m_requests->end();
  for(; request != end; ++request)
  {
    request->timer->Stop();
  }
}

template<typename R, typename K>
void ResourceCache<R,K>::Timeout(AsioExpress::Error error, ResourceItemPointer item)
{
  ReturnError(error, item);
}

template<typename R, typename K>
void ResourceCache<R,K>::ReturnError(AsioExpress::Error error, ResourceItemPointer item)
{
  typename Requests::iterator request 
    = std::find(m_requests->begin(), m_requests->end(), item);
  if (request != m_requests->end())
  {
    CompletionHandler handler(request->completionHandler);
    m_requests->erase(request);
    if (error)
      handler(error);
    else
      handler(Error(AsioExpress::ErrorCode::ResourceCacheTimeout));
  }
}
template<typename R, typename K>
bool ResourceCache<R,K>::CanDelete(Request const & request, Key key)
{
  return request.key == key;
}

} // namespace AsioExpress
