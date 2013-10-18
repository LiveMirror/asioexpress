//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpress/Testing/HippoMocks.h"
#include "AsioExpress/CompletionHandler.hpp"
#include "AsioExpress/Handles/PointerHandle.h"

namespace HippoMocks {

template <typename T>
struct PointerValueParam
{
	explicit PointerValueParam(T const & value): value(value)
	{
	}
	T const & value;
};

template <typename T>
PointerValueParam<T> PointerValue(T& t) { return PointerValueParam<T>(t); }

template <typename T>
struct comparer<boost::shared_ptr<T> const &>
{ 
	static inline bool compare(typename PointerValueParam<T> a, typename boost::shared_ptr<T> const & b)
	{
		return a.value == *b;
	}
	static inline bool compare(typename boost::shared_ptr<T> a, typename boost::shared_ptr<T> const & b)
	{
		return a == b;
	}
	static inline bool compare(DontCare, typename boost::shared_ptr<T> const &)
	{
		return true;
	}
};

template <typename T>
struct comparer<boost::shared_ptr<T> >
{ 
	static inline bool compare(typename PointerValueParam<T> a, typename boost::shared_ptr<T> b)
	{
		return a.value == *b;
	}
	static inline bool compare(typename boost::shared_ptr<T> a, typename boost::shared_ptr<T> b)
	{
		return a == b;
	}
	static inline bool compare(DontCare, typename boost::shared_ptr<T>)
	{
		return true;
	}
};

template <typename T>
struct comparer<T*>
{ 
	static inline bool compare(typename PointerValueParam<T> a, typename T* b)
	{
		return a.value == *b;
	}
	static inline bool compare(typename T* a, typename T* b)
	{
		return a == b;
	}
	static inline bool compare(DontCare, typename T*)
	{
		return true;
	}
};

template <>
struct printArg<AsioExpress::CompletionHandler>
{
  static inline void print(std::ostream &os, AsioExpress::CompletionHandler arg, bool withComma)
  {
    if (withComma) os << ",";
    os << &arg;
  }

};

template <typename T>
struct do_assign<OutParam<T>, boost::shared_ptr<T>, true>
{
  static void assign_to(OutParam<T> outparam, boost::shared_ptr<T> refparam)
  {
    *refparam = outparam.value;
  }
  static void assign_from(OutParam<T> inparam, boost::shared_ptr<T> refparam)
  {
    inparam.value = *refparam;
  }
};

template <typename T>
struct do_assign<OutParam<T>, boost::shared_ptr<T> &, true>
{
  static void assign_to(OutParam<T> outparam, boost::shared_ptr<T> & refparam)
  {
    *refparam = outparam.value;
  }
  static void assign_from(OutParam<T> inparam, boost::shared_ptr<T> & refparam)
  {
    inparam.value = *refparam;
  }
};

template <typename A, typename B>
struct with_ref<boost::shared_ptr<A>const&, const OutParam<B> &>
{
  typedef OutParam<B> type;
};

template <typename T>
struct do_assign<OutParam<T>, boost::shared_ptr<T> const&, true>
{
  static void assign_to(OutParam<T> outparam, boost::shared_ptr<T> const& refparam)
  {
    *refparam = outparam.value;
  }
  static void assign_from(OutParam<T> inparam, boost::shared_ptr<T> const& refparam)
  {
    inparam.value = *refparam;
  }
};

template <typename T>
struct do_assign<OutParam<T>, AsioExpress::PointerHandle<T>, true>
{
  static void assign_to(OutParam<T> outparam, AsioExpress::PointerHandle<T> refparam)
  {
    refparam.set(outparam.value);
  }
  static void assign_from(OutParam<T> outparam, AsioExpress::PointerHandle<T> refparam)
  {
    outparam.value = refparam.get();
  }
};

template <typename T>
struct do_assign<InParam<T,false>, T, true>
{
  static void assign_to(InParam<T,false> inparam, T& value)
  {
    value = inparam.value;
  }
  static void assign_from(InParam<T,false> inparam, T& value)
  {
    inparam.value = value;
  }
};

template <typename T>
struct do_assign<InParam<T,false>, boost::shared_ptr<T>, true>
{
  static void assign_to(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    *refparam = inparam.value;
  }
  static void assign_from(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    inparam.value = *refparam;
  }
};


template <typename T>
struct do_assign<InParam<T,false>, boost::shared_ptr<T> &, true>
{
  static void assign_to(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    *refparam = inparam.value;
  }
  static void assign_from(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    inparam.value = *refparam;
  }
};


template <typename T>
struct do_assign<InParam<T,false>, boost::shared_ptr<T> const&, true>
{
  static void assign_to(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    *refparam = inparam.value;
  }
  static void assign_from(InParam<T,false> inparam, boost::shared_ptr<T> refparam)
  {
    inparam.value = *refparam;
  }
};

template <typename T1, typename T2>
struct do_assign<T1, T2, true>
{
  static void assign_from(T1 inparam, T2 refparam)
  {
    inparam.value = refparam;
  }
};
} // namespace HippoMocks

using HippoMocks::PointerValue;
