//
// Coroutine.h
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

namespace AsioExpress {

class Coroutine
{
public:
  Coroutine() : value_(0) {}
  bool IsChild() const { return value_ < 0; }
  bool IsParent() const { return !IsChild(); }
  bool IsComplete() const { return value_ == -1; }
  int GetCurrentLine() const { return value_ == -1 ? value_ : (value_ < 0 ? -value_ : value_); }
private:
  friend class coroutine_ref;
  int value_;
};

class coroutine_ref
{
public:
  coroutine_ref(Coroutine& c) : value_(c.value_), modified_(false) {}
  coroutine_ref(Coroutine* c) : value_(c->value_), modified_(false) {}
  ~coroutine_ref() { if (!modified_) value_ = -1; }
  operator int() const { return value_; }
  int& operator=(int v) { modified_ = true; return value_ = v; }
private:
  void operator=(const coroutine_ref&);
  int& value_;
  bool modified_;
};

} // namespace AsioExpress
