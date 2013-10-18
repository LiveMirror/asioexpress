//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AsioExpressTest/pch.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/shared_ptr.hpp"
#include "AsioExpress/Testing/HippoMockExtensions.hpp"

class MyClass
{
public:
  MyClass() : x(0), y(0)
  {
  }
  int x;
  int y;
  bool operator==(MyClass const & that) const
  {
    return x == that.x && y == that.y;
  }
};

typedef boost::shared_ptr<MyClass> MyClassPointer;

typedef boost::shared_ptr<int> IntPointer;

class MyInterface
{
public:
    virtual ~MyInterface() {};

    virtual void MyMethod1(IntPointer x) = 0;
    virtual void MyMethod2(MyClassPointer x) = 0;
    virtual void MyMethod3(MyClass *x) = 0;
    virtual void MyMethod4(IntPointer const & x) = 0;
};

BOOST_AUTO_TEST_SUITE(HippoMockExtensionsTest)

void TestObjectCompare()
{
  using namespace AsioExpress;
  using namespace std;

  MockRepository mocks;
  MyInterface * m = mocks.Mock<MyInterface>();

  IntPointer      pi = IntPointer(new int(1));
  MyClassPointer  pc = MyClassPointer(new MyClass);
  MyClass*        raw_pc = new MyClass;

  int x = 1;
  MyClass c;


  mocks.ExpectCall(
    m,
    MyInterface::MyMethod1).With(pi);
  mocks.ExpectCall(
    m,
    MyInterface::MyMethod1).With(PointerValue(x));


  mocks.ExpectCall(
    m,
    MyInterface::MyMethod2).With(pc);
  mocks.ExpectCall(
    m,
    MyInterface::MyMethod2).With(PointerValue(c));


  mocks.ExpectCall(
    m,
    MyInterface::MyMethod3).With(raw_pc);
  mocks.ExpectCall(
    m,
    MyInterface::MyMethod3).With(PointerValue(c));


  mocks.ExpectCall(
    m,
    MyInterface::MyMethod4).With(pi);
  mocks.ExpectCall(
    m,
    MyInterface::MyMethod4).With(PointerValue(x));


  m->MyMethod1(pi);
  m->MyMethod1(pi);    

  m->MyMethod2(pc);    
  m->MyMethod2(pc);    

  m->MyMethod3(raw_pc);    
  m->MyMethod3(raw_pc); 

  m->MyMethod4(pi);    
  m->MyMethod4(pi);    
}

BOOST_AUTO_TEST_SUITE_END()
