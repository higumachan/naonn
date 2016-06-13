#ifndef NAONN_VARIABLE_HPP
#define NAONN_VARIABLE_HPP

#include "TypeHelper.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace variable
{

template<
    typename Type,
    int ID,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
class Variable
{
public:
  static const int id = ID;

  Variable(const Type& _value)
      : value(_value)
  { }

  void set_value(const Type& _value)
  { value = _value; }

  Type get_value() const
  { return value; }

  Type get_grad(const Variable<Type, ID, TypeHelper>& target) const
  {
    return TypeHelper::one;
  }

  template<typename OtherType, int OTHER_ID, typename OtherTypeHelper>
  virtual Type get_grad(const Variable<OtherType, OTHER_ID, OtherTypeHelper>& target) const
  {
    return (TypeHelper::zero);
  }

private:
  Type value;
};

namespace
{
const int CONST_VARIABLE_ID = -1;
}

template<
    typename Type,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
class ConstVariable
    : public Variable<Type, CONST_VARIABLE_ID>
{
public:
  ConstVariable(const Type& _value)
      : Variable(_value)
  { }

  template<typename OtherType, int OTHER_ID, typename OtherTypeHelper>
  virtual Type get_grad(const Variable<OtherType, OTHER_ID, OtherTypeHelper>& target) const
  {
    return TypeHelper::zero;
  }

private:
  Type value;
};

}
}
}
}

#endif
