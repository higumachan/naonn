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
      const variable::Variable<TargetType, ID>& target,
      const Left& left,
      const Right& right
  )
  {
    return (Derived::grad_detail_left(left.get_value(), right.get_value()) *
        left.get_grad(target) +
        Derived::grad_detail_right(left.get_value(), right.get_value()) *
            right.get_grad(target));
  }

  using result_of_apply = typename std::result_of<decltype(apply)>::type;
  using result_of_grad = typename std::result_of<decltype(grad)>::type;

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
    : public ApplyBinaryFunctionBase<
        max<InputTypeLeft, InputTypeRight, OutputType>,
        InputTypeLeft,
        InputTypeRight,
        OutputType
    >
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
    : public ApplyBinaryFunctionBase<
        min<InputTypeLeft, InputTypeRight, OutputType>,
        InputTypeLeft,
        InputTypeRight,
        OutputType
    >
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
    : public ApplyBinaryFunctionBase<
        plus<InputTypeLeft, InputTypeRight, OutputType>,
        InputTypeLeft,
        InputTypeRight,
        OutputType
    >
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return x + y; }

  static OutputType grad_detail(const InputTypeLeft&, const InputTypeRight&)
  { return 1; }
};

template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
struct multiply
    : public ApplyBinaryFunctionBase<
        multiply<InputTypeLeft, InputTypeRight, OutputType>,
        InputTypeLeft,
        InputTypeRight,
        OutputType
    >
{
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return x * y; }

  static OutputType grad_detail_left(const InputTypeLeft&, const InputTypeRight& y)
  { return y; }

  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight&)
  { return x; }
};

// }}}
}

// {{{ DEFINE_BINARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE)
#define DEFINE_BINARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE) \
template< \
  typename Left, \
  typename Right \
> \
expressions::BinaryExpressions< \
  expressions::BaseExpressions<Left>, \
  OPERATOR_TYPE,  \
  expressions::BaseExpressions<Right> \
> OPERATOR_NAME(const Left& left, const Right& right) \
{ \
  expressions::BinaryExpressions< \
    expressions::BaseExpressions<Left>, \
    OPERATOR_TYPE, \
    BaseExpressions<Right> \
  >(left, right); \
} \
\
template< \
  typename Left, \
  typename Type \
> \
expressions::BinaryExpressions< \
  expressions::BaseExpressions<Left>, \
  OPERATOR_TYPE, \
  variable::ConstVariable<Type>\
> OPERATOR_NAME(const Left& left, const Type& right) \
{ \
  expressions::BinaryExpressions< \
    expressions::BaseExpressions<Left>, \
    OPERATOR_TYPE, \
    variable::ConstVariable<Type> \
  >(left, variable::ConstVariable<Type>(right)); \
} \
\
template< \
  typename Right, \
  typename Type \
> \
expressions::BinaryExpressions< \
  variable::ConstVariable<Type>, \
  OPERATOR_TYPE, \
  expressions::BaseExpressions<Right>\
> OPERATOR_NAME(const Type& left, const Right& right) \
{ \
  expressions::BinaryExpressions<\
    variable::ConstVariable<Type>, \
    OPERATOR_TYPE, \
    expressions::BaseExpressions<Right> \
  >(variable::ConstVariable<Type>(left), right); \
}

//}}}

DEFINE_BINARY_OPERATORS(operator+, basics::plus)

DEFINE_BINARY_OPERATORS(operator*, basics::multiply)

DEFINE_BINARY_OPERATORS(min, basics::min)

DEFINE_BINARY_OPERATORS(max, basics::max)

}
}
}
}

#endif
