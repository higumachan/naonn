#ifndef NAONN_BINARY_OPERATORS_HPP
#define NAONN_BINARY_OPERATORS_HPP

#include <type_traits>
#include "Variable.hpp"
#include "BinaryExpressions.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace operators
{

//{{{ ApplyBinaryFunctionBase
template<typename Derived, typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct ApplyBinaryFunctionBase
{
  Derived& derived()
  { return static_cast<Derived&>(*this); }

  const Derived& derived() const
  { return static_cast<Derived&>(*this); }


  static OutputType apply(InputTypeLeft left, InputTypeRight right)
  {
    return Derived::apply_detail(left, right);
  }

  template<typename Left, typename Right, typename TargetType, int ID>
  static OutputType grad(
      const variable::Variable<TargetType, ID>& target, const Left& left,
      const Right& right
  )
  {
    return (Derived::grad_detail_left(left.get_value(), right.get_value()) *
        left.get_grad(target) +
        Derived::grad_detail_right(left.get_value(), right.get_value()) *
            right.get_grad(target));
  }

  using result_of_apply = std::result_of<decltype(apply)>::type;
  using result_of_grad = std::result_of<decltype(grad)>::type;

  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y);

  static OutputType grad_detail(const InputTypeLeft& x, const InputTypeRight& y);

  static OutputType grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return Derived::grad_detail(x, y);
  }

  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return Derived::grad_detail(x, y);
  }

};
// }}}


namespace basics
{
// {{{ basic operators
template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct max
    : public ApplyBinaryFunctionBase<max, InputTypeLeft, InputTypeRight, OutputType>
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x > y ? x : y;
  }

  static OutputType grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x > y ? 1 : 0;
  }

  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x < y ? 1 : 0;
  }
};

template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct min
    : public ApplyBinaryFunctionBase<min, InputTypeLeft, InputTypeRight, OutputType>
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x < y ? x : y;
  }

  static OutputType grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x < y ? 1 : 0;
  }

  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x > y ? 1 : 0;
  }
};

template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct plus
    : public ApplyBinaryFunctionBase<plus, InputTypeLeft, InputTypeRight, OutputType>
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return x + y; }

  static OutputType grad_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return 1; }
};

template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct multiply
    : public ApplyBinaryFunctionBase<multiply, InputTypeLeft, InputTypeRight, OutputType>
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return x * y; }

  static OutputType grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  { return y; }

  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  { return x; }
};

// }}}
}

// {{{ DEFINE_BINARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE)
#define DEFINE_BINARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE) \
template< \
  template Left, \
  template Right \
> \
expressions::BinaryExpressions<Left, basics::OPERATOR_TYPE, Right> OPERATOR_NAME(const Left& left, const Right& right) \
{ \
  expressions::BinaryExpressions<Left, basics::OPERATOR_TYPE, Right>(left, right); \
} \
template< \
  template Left, \
  template Type \
> \
expressions::BinaryExpressions<Left, basics::OPERATOR_TYPE, ConstVariable<Type>> OPERATOR_NAME(const Left& left, const Type& right) \
{ \
  expressions::BinaryExpressions<Left, basics::OPERATOR_TYPE, ConstVariable<Type>>(left, ConstVariable<Type>(right)); \
} \
template< \
  template Right, \
  template Type \
> \
expressions::BinaryExpressions<ConstVariable<Type>, basics::OPERATOR_TYPE, Right> OPERATOR_NAME(const Type& left, const Right& right) \
{ \
  expressions::BinaryExpressions<ConstVariable<Type>, basics::OPERATOR_TYPE, Right>(ConstVariable<Type>(left), right); \
}

//}}}

DEFINE_BINARY_OPERATORS(operator+, plus)

DEFINE_BINARY_OPERATORS(operator*, multiply)

DEFINE_BINARY_OPERATORS(min, min)

DEFINE_BINARY_OPERATORS(max, max)

}
}
}
}

#endif
