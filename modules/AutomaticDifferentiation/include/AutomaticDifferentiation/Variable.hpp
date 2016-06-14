#ifndef NAONN_VARIABLE_HPP
#define NAONN_VARIABLE_HPP

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace variable
{

using id_type = int;

}
}
}
}

#include "BaseExpression.hpp"
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
    id_type ID,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
class Variable
    : expressions::BaseExpression<
        Variable<Type, ID, TypeHelper>,
        Type, Type
    >
{
public:
  static constexpr id_type id = ID;

  using value_type = Type;
  using grad_type = Type;

  Variable(const Type& _value)
      : value(_value)
  { }

  void set_value(const Type& _value)
  { value = _value; }

  value_type get_value() const
  { return value; }

  grad_type get_grad(const Variable<Type, ID, TypeHelper>&) const
  {
    return TypeHelper::one();
  }

  template<typename OtherType, id_type OTHER_ID, typename OtherTypeHelper>
  grad_type get_grad(const Variable<OtherType, OTHER_ID, OtherTypeHelper>&) const
  {
    return (TypeHelper::zero());
  }

private:
  value_type value;
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
    : public Variable<Type, CONST_VARIABLE_ID, TypeHelper>
{
public:
  ConstVariable(const Type& _value)
      : Variable<Type, CONST_VARIABLE_ID, TypeHelper>(_value)
  { }

  template<typename OtherType, id_type OTHER_ID, typename OtherTypeHelper>
  Type get_grad(const Variable<OtherType, OTHER_ID, OtherTypeHelper>&) const
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


#define NYAO_NN_VARIABLE(type) nyao::differentiation::automatic::variable::Variable<type, __COUNTER__>

#endif
