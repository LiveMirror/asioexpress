//               Copyright Ross MacGregor 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace AsioExpress {

class SimpleStateMachine
{
public:
  typedef int StepType;

  SimpleStateMachine();

  StepType CurrentStep();

  void Forward();

  void Back();

  void FirstStep();

  void SetStep(StepType n);

  void SetParent();

  bool IsParent();

  bool IsChild();

protected:
  ~SimpleStateMachine() {}

private:
  StepType  m_step;
  bool      m_parent;
};

inline SimpleStateMachine::SimpleStateMachine() :
  m_step(1),
  m_parent(false)
{
}

inline SimpleStateMachine::StepType SimpleStateMachine::CurrentStep()
{
  return m_step;
}

inline void SimpleStateMachine::Forward()
{
  ++m_step;
}

inline void SimpleStateMachine::Back()
{
  --m_step;
}

inline void SimpleStateMachine::FirstStep()
{
  m_step = 1;
}

inline void SimpleStateMachine::SetStep(StepType n)
{
  m_step = n;
}

inline void SimpleStateMachine::SetParent()
{
  m_parent = true;
}

inline bool SimpleStateMachine::IsParent()
{
  return m_parent;
}

inline bool SimpleStateMachine::IsChild()
{
  return !m_parent;
}

} // namespace AsioExpress
