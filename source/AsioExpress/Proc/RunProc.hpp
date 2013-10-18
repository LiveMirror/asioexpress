//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsioExpressError/CatchMacros.hpp"
#include "AsioExpress/Proc/Context.hpp"

namespace AsioExpress {
namespace Proc {

// Async Processor for a 0 argument coroutine.
template<class F>
class AsyncProcessor0
{
public:
    AsyncProcessor0(F f, Context context) :
        context(context),
        f(f)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f;
};

template<class F>
void RunProc(F f, Context context)
{
  context.Start();

  try
  {
    f(context);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F>
AsioExpress::CompletionHandler PushProc(F f, Context context)
{
    AsyncProcessor0<F> proc(f,context);
    return proc;
}

// Async Processor for a 1 argument coroutine.
template<class F, class P1>
class AsyncProcessor1
{
public:
    AsyncProcessor1(F f, Context context, P1 p1) :
        context(context),
        f(f), p1(p1)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1;
};

template<class F, class P1>
void RunProc(F f, Context context, P1 p1)
{
  context.Start();

  try
  {
    f(context, p1);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1)
{
    AsyncProcessor1<F, P1> proc(f,context, p1);
    return proc;
}

// Async Processor for a 2 argument coroutine.
template<class F, class P1, class P2>
class AsyncProcessor2
{
public:
    AsyncProcessor2(F f, Context context, P1 p1, P2 p2) :
        context(context),
        f(f), p1(p1), p2(p2)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2;
};

template<class F, class P1, class P2>
void RunProc(F f, Context context, P1 p1, P2 p2)
{
  context.Start();

  try
  {
    f(context, p1, p2);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2)
{
    AsyncProcessor2<F, P1, P2> proc(f,context, p1, p2);
    return proc;
}

// Async Processor for a 3 argument coroutine.
template<class F, class P1, class P2, class P3>
class AsyncProcessor3
{
public:
    AsyncProcessor3(F f, Context context, P1 p1, P2 p2, P3 p3) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3;
};

template<class F, class P1, class P2, class P3>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3)
{
    AsyncProcessor3<F, P1, P2, P3> proc(f,context, p1, p2, p3);
    return proc;
}

// Async Processor for a 4 argument coroutine.
template<class F, class P1, class P2, class P3, class P4>
class AsyncProcessor4
{
public:
    AsyncProcessor4(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3), p4(p4)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3, p4);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3; P4 p4;
};

template<class F, class P1, class P2, class P3, class P4>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3, p4);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3, class P4>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4)
{
    AsyncProcessor4<F, P1, P2, P3, P4> proc(f,context, p1, p2, p3, p4);
    return proc;
}

// Async Processor for a 5 argument coroutine.
template<class F, class P1, class P2, class P3, class P4, class P5>
class AsyncProcessor5
{
public:
    AsyncProcessor5(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3, p4, p5);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3; P4 p4; P5 p5;
};

template<class F, class P1, class P2, class P3, class P4, class P5>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3, p4, p5);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3, class P4, class P5>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
    AsyncProcessor5<F, P1, P2, P3, P4, P5> proc(f,context, p1, p2, p3, p4, p5);
    return proc;
}

// Async Processor for a 6 argument coroutine.
template<class F, class P1, class P2, class P3, class P4, class P5, class P6>
class AsyncProcessor6
{
public:
    AsyncProcessor6(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3, p4, p5, p6);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3; P4 p4; P5 p5; P6 p6;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3, p4, p5, p6);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3, class P4, class P5, class P6>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
    AsyncProcessor6<F, P1, P2, P3, P4, P5, P6> proc(f,context, p1, p2, p3, p4, p5, p6);
    return proc;
}

// Async Processor for a 7 argument coroutine.
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class AsyncProcessor7
{
public:
    AsyncProcessor7(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6), p7(p7)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3, p4, p5, p6, p7);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3; P4 p4; P5 p5; P6 p6; P7 p7;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3, p4, p5, p6, p7);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
    AsyncProcessor7<F, P1, P2, P3, P4, P5, P6, P7> proc(f,context, p1, p2, p3, p4, p5, p6, p7);
    return proc;
}

// Async Processor for a 8 argument coroutine.
template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class AsyncProcessor8
{
public:
    AsyncProcessor8(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) :
        context(context),
        f(f), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6), p7(p7), p8(p8)
    {
    }

    void operator()(AsioExpress::Error error = AsioExpress::Error())
    {
      CHECK_MSG(!context.IsComplete(), "Async. processor invoked but it has already completed.");
      
      if (error)
      {
        context.Complete(error);
        return;
      }

      context.Start();

      try
      {
        f(context, p1, p2, p3, p4, p5, p6, p7, p8);
        context.Exit();
      }
      ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
    }

private:
    Context context;
    F f; P1 p1; P2 p2; P3 p3; P4 p4; P5 p5; P6 p6; P7 p7; P8 p8;
};

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
void RunProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
  context.Start();

  try
  {
    f(context, p1, p2, p3, p4, p5, p6, p7, p8);
    context.Exit();
  }
  ASIOEXPRESS_CATCH_ERROR_AND_DO( context.Complete(error) );
}

template<class F, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
AsioExpress::CompletionHandler PushProc(F f, Context context, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
    AsyncProcessor8<F, P1, P2, P3, P4, P5, P6, P7, P8> proc(f,context, p1, p2, p3, p4, p5, p6, p7, p8);
    return proc;
}

} // namespace Proc
} // namespace AsioExpress
