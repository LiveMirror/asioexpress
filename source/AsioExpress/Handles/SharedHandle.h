//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#pragma warning(push)
#pragma warning(disable: 4244)
#include "boost/shared_ptr.hpp"
#pragma warning(pop)

#include "ErrorHandling/Check.h"

namespace AsioExpress {

///
/// A non-null reference counted smart pointer. 
/// It is based upon boost::shared_ptr.
/// 
template <class X> 
class SharedHandle
{
    template<class Y>
    friend class SharedHandle;

    template<class T, class U> 
    friend bool 
    operator<(SharedHandle<T> const & a, SharedHandle<U> const & b);

    template<class T>
    friend void 
    swap(SharedHandle<T> & a, SharedHandle<T> & b);

    template<class T, class U>
    friend SharedHandle<T>
    dynamic_pointer_cast(SharedHandle<U> const & r);

public:
    typedef X element_type;

    ///
    /// Constructs a shared handle that owns the pointer p. 
    /// @param p The input pointer must not be null or an contract exception 
    /// will be thrown. It must be convertible to X *. Y must be a complete
    /// type. The expression delete p must be well-formed, must not invoke 
    /// undefined behavior, and must not throw exceptions.
    ///
    template<class Y> 
    explicit 
    SharedHandle(Y * p) : 
        sharedPointer(p) 
    {
        CHECK_MSG(p != 0, "SharedHandle requires a non-null pointer.");
    }

    ///
    /// Constructs a shared handle that owns the pointer p and the deleter d. 
    /// @param p The input pointer must not be null or an contract exception 
    /// will be thrown. It must be convertible to X *. Y must be a complete
    /// type. The expression delete p must be well-formed, must not invoke 
    /// undefined behavior, and must not throw exceptions.
    /// @param d Deletion function or functor d(p) is called when the object
    /// needs to be deleted.
    ///
    template<class Y, class D> 
    explicit 
    SharedHandle(Y * p, D d) : 
        sharedPointer(p, d) 
    {
        CHECK_MSG(p != 0, "SharedHandle requires a non-null pointer.");
    }

    ///
    /// Constructs a shared handle that shares ownership with r.
    /// @param r Another shared handle.
    ///
    SharedHandle(SharedHandle const & r) :
        sharedPointer(r.sharedPointer)
    {
    }

    ///
    /// Constructs a shared handle that shares ownership with r.
    /// @param r Another shared handle.
    ///
    template<class Y>
    SharedHandle(SharedHandle<Y> const & r) :
        sharedPointer(r.sharedPointer)
    {
    }

    ///
    /// Constructs a shared handle that shares ownership with r.
    /// @param r A nullable shared handle.
    ///
    template<class Y> 
    explicit
    SharedHandle(boost::shared_ptr<Y> const & r) :
        sharedPointer(r)
    {
        CHECK_MSG(r, "SharedHandle requires non-null initialization.");
    }

    ///
    /// Assignment operator
    /// @param r Another shared handle.
    /// @return Returns *this.
    ///
    SharedHandle & operator=(SharedHandle const & r)
    {
        sharedPointer.operator=(r.sharedPointer);
        return *this;
    }

    ///
    /// Assignment operator
    /// @param r Another shared handle.
    /// @return Returns *this.
    ///
    template<class Y>
    SharedHandle & operator=(SharedHandle<Y> const & r)
    {
        sharedPointer.operator=(r.sharedPointer);
        return *this;
    }

    ///
    /// Assignment operator
    /// @param r A nullable shared handle.
    /// @return Returns *this.
    ///
    template<class Y>
    SharedHandle & operator=(boost::shared_ptr<Y> const & r)
    {
        CHECK_MSG(r, "SharedHandle requires non-null assignment.");
        sharedPointer.operator=(r);
        return *this;
    }

    ///
    /// Dereference operator.
    /// @return Returns a reference to the object associated with 
    /// the shared handle.
    /// 
    X & operator*() const    
    {
        return sharedPointer.operator*();
    }

    ///
    /// Pointer dereference operator.
    /// @return Returns a pointer to the object associated with 
    /// the shared handle.
    /// 
    X * operator->() const
    { 
        return sharedPointer.operator->();
    }

    ///
    /// Explicit pointer dereference method.
    /// @return Returns a pointer to the object associated with 
    /// the shared handle.
    /// 
    X * get() const
    { 
        return sharedPointer.get();
    }

    ///
    /// Gets a boost shared pointer to the object associated with 
    /// the shared handle.
    /// @return Returns a boost::shared_ptr<X>. 
    ///
    boost::shared_ptr<X> get_shared_ptr()
    {
        return sharedPointer;
    }

    ///
    /// Indicates of the use count for the shared object is one. 
    ///
    bool unique() const 
    {
        return sharedPointer.unique();
    }

    ///
    /// Returns the use count for the shared objects.
    /// @note This function name goes againt the established naming conventions but
    /// was choosen to be compatible with boost::shared_ptr which SharedHandle
    /// emulates.
    ///
    long use_count() const
    {
        return sharedPointer.use_count();
    }

private:
    SharedHandle();

    boost::shared_ptr<X> sharedPointer;
};

template<class T, class U> 
inline bool 
operator==(AsioExpress::SharedHandle<T> const & a, AsioExpress::SharedHandle<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U>
inline bool 
operator!=(AsioExpress::SharedHandle<T> const & a, AsioExpress::SharedHandle<U> const & b)
{
    return a.get() != b.get();
}

template<class T, class U> 
inline bool 
operator<(AsioExpress::SharedHandle<T> const & a, AsioExpress::SharedHandle<U> const & b)
{
    return a.sharedPointer._internal_less(b.sharedPointer);
}

template<class T>
inline void 
swap(AsioExpress::SharedHandle<T> & a, AsioExpress::SharedHandle<T> & b)
{
    a.sharedPointer.swap(b.sharedPointer);
}

template<class T, class U>
AsioExpress::SharedHandle<T> 
dynamic_pointer_cast(AsioExpress::SharedHandle<U> const & r)
{
    boost::shared_ptr<T> sharedPointer(dynamic_pointer_cast<T,U>(r));
    return SharedHandle<T>(sharedPointer.get()) 
}

// get_pointer() enables boost::mem_fn to recognize SharedHandle
template<class T>
inline T * 
get_pointer(SharedHandle<T> const & p)
{
    return p.get();
}

// operator<<
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, SharedHandle<Y> const & p)
{
    os << p.get();
    return os;
}

} // namespace AsioExpress
