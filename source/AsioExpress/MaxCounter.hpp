//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <AsioExpressError/Check.hpp>

namespace AsioExpress {

template <typename T>
class MaxCounter
{
public:
    typedef T SizeType;

    MaxCounter(SizeType maxCount) :
        count(0),
        maxCount(maxCount)
    {
    }

    SizeType GetCount()
    {
        return count;
    }

    SizeType GetMaxCount()
    {
        return maxCount;
    }

    bool isMax()
    {
        return count == maxCount;
    }

    SizeType & operator++()
    {
        CHECK(! isMax());
        ++count;
    }

    SizeType & operator--()
    {
        CHECK(count != 0);
        --count;
    }

private:
    SizeType count;
    SizeType maxCount;
};

}// namespace AsioExpress
