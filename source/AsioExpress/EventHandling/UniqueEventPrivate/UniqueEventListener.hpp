//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "AsioExpressError/CallStack.hpp"
#include "AsioExpressError/Check.hpp"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/ErrorCodes.hpp"
#include "AsioExpress/Timer/Timer.hpp"

namespace AsioExpress {
namespace UniqueEventPrivate {

template<typename E>
class UniqueEventListener :
  public boost::enable_shared_from_this<UniqueEventListener<E> >
{
public:
  typedef E Event;
  typedef boost::shared_ptr<Event> EventPointer;
  typedef boost::shared_ptr<UniqueEventListener> Pointer;

  UniqueEventListener(
      EventPointer eventValue) :
    m_eventValue(eventValue),
    m_assignedEventData(false),
    m_isComplete(false)
  {
  }

  void AsyncWait(
      TimerPointer const & waitTimer,
      CompletionHandler completionHandler);

  void AddEvent(
      Event eventValue);

  void Cancel();

  bool IsActive() const;

  static void Timeout(
      AsioExpress::Error error,
      typename UniqueEventListener::Pointer listener);

private:
  EventPointer        m_eventValue;
  CompletionHandler   m_completionHandler;
  TimerPointer        m_timer;
  bool                m_assignedEventData;
  bool                m_isComplete;
};

template<typename E>
inline void UniqueEventListener<E>::AsyncWait(
    TimerPointer const & waitTimer,
    CompletionHandler completionHandler)
{
  CHECK(waitTimer);
  CHECK(!completionHandler.empty());

  CHECK_MSG(!m_isComplete, "UniqueEvent::AsyncWait called on a listener that has already completed.");
  CHECK_MSG(!m_completionHandler, "UniqueEvent::AsyncWait called twice for the same listener.");

  // If event data has already been assigned then call completion handler
  // immediately with success status.
  if (m_assignedEventData)
  {
    m_isComplete = true;
    completionHandler(Error());
    return;
  }

  // Wait for event to occur.
  //
  m_completionHandler = completionHandler;
  m_timer = waitTimer;
  m_timer->AsyncWait(
    boost::bind(&UniqueEventListener::Timeout, _1, this->shared_from_this()));
}

template<typename E>
inline void UniqueEventListener<E>::AddEvent(
    Event eventValue)
{
  if (m_isComplete)
    return;

  *(m_eventValue) = eventValue;
  m_assignedEventData = true;

  // Has an AsyncWait been called?
  if (m_completionHandler)
  {
    m_isComplete = true;
    m_completionHandler(Error());
    m_timer->Stop();
    m_completionHandler = 0;
  }
}

template<typename E>
inline void UniqueEventListener<E>::Cancel()
{
  if (m_isComplete)
    return;

  m_isComplete = true;

  if (m_timer)
  {
    m_completionHandler(AsioExpress::Error(boost::asio::error::operation_aborted));
    m_timer->Stop();
    m_completionHandler = 0;
  }
}

template<typename E>
inline bool UniqueEventListener<E>::IsActive() const
{
  return !m_isComplete;
}

template<typename E>
void UniqueEventListener<E>::Timeout(
    AsioExpress::Error error,
    typename UniqueEventListener::Pointer listener)
{
  if (listener->m_isComplete)
      return;

  // If not complete there must be a completion handler.
  CHECK(!listener->m_completionHandler.empty());

  // Convert to timeout error if timer has expired.
  if (!error)
    error = Error(ErrorCode::UniqueEventTimeout);

  listener->m_isComplete = true;
  listener->m_completionHandler(error);
  listener->m_completionHandler = 0;
}

} // namespace UniqueEventPrivate
} // namespace AsioExpress
