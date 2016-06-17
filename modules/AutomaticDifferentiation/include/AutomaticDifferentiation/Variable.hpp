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

#include "Expression.hpp"
#include "TypeHelper.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace variable
{

namespace impl
{

template<
    typename Type,
    id_type ID,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
class Variable
{
public:
  static constexpr id_type id = ID;

  using value_type = Type;
  using grad_type = Type;

  Variable(const Type& _value)
  {
    value = new Type;
    *value = _value;
    is_original = true;
  }

  Variable(const Variable<Type, ID, TypeHelper>& other)
    : value(other.value)
  {
    is_original = false;
  }

  ~Variable()
  {
    if (is_original) {
      delete value;
    }
  }

  void set_value(const Type& _value)
  { *value = _value; }

  value_type get_value() const
  { return *value; }

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
  value_type* value;
  bool is_original = false;
};

namespace
{
const int CONST_VARIABLE_ID = -1;
}


template<
    typename Type,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
struct ConstVariable
{
  using value_type = Type;
  using grad_type = Type;

  static_assert(!std::is_reference<value_type>::value, "reference");

  ConstVariable(const Type& _value)
  {
    value = _value;
  }

  value_type get_value() const
  { return value; }

  template<int ID>
  grad_type get_grad(const Variable<Type, ID, TypeHelper>&) const
  {
    return TypeHelper::zero();
  }

private:
  value_type value;
};

} //impl

template<
    typename Type,
    id_type ID,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
using Variable = expressions::Expression<impl::Variable<Type, ID, TypeHelper>>;

template<
    typename Type,
    typename TypeHelper=type_helper::TypeHelper<Type>
>
using ConstVariable = expressions::Expression<impl::ConstVariable<Type, TypeHelper>>;

}
}
}
}


#define NYAO_NN_VARIABLE(type) nyao::differentiation::automatic::variable::Variable<type, __COUNTER__>

#endif
