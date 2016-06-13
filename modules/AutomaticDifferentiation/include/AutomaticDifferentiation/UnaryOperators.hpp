#ifndef NAONN_UNARY_OPERATORS_HPP
#define NAONN_UNARY_OPERATORS_HPP

#include <type_traits>
#include "Variable.hpp"
#include "UnaryExpressions.hpp"

namespace nyao {
namespace differentiation {
namespace automatic {
namespace operators {

//{{{ ApplyUnaryFunctionBase
template <
    typename Derived,
    typename InputType,
    typename OutputType
>
struct ApplyUnaryFunctionBase {
  Derived &derived() { return static_cast<Derived &>(*this); }

  const Derived &derived() const { return static_cast<Derived &>(*this); }


  static OutputType apply(const InputType& operand) {
    return Derived::apply_detail(operand);
  }

  template <typename TargetType, int ID>
  static OutputType grad(const variable::Variable<TargetType, ID> &target, const InputType &operand) {
    return Derived::grad_detail(operand.get_value()) * operand.get_grad(target);
  }
  using result_of_apply = std::result_of<decltype(apply)>::type;
  using result_of_grad = std::result_of<decltype(grad)>::type;

  static OutputType grad_detail(const InputType& x);
  static OutputType apply_detail(const InputType& x);
};

//}}}

namespace basics
{
//{{{ basic unary operators

template <
  typename Type,
  typename TypeHelper=type_helper::TypeHelper<Type>
>
struct nagate : public ApplyUnaryFunctionBase<nagate, Type, Type> {
  static Type apply_detail(const Type& x) { return -x; }
  static Type grad_detail(const Type& x) { return -TypeHelper::one; }
};

template <
  typename InputType,
  typename OutputType,
  typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct sin : public ApplyUnaryFunctionBase<sin, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return InputTypeHelper::sin(x); }
  static OutputType grad_detail(const InputType& x) { return InputTypeHelper::cos(x); }
};

template <
    typename InputType,
    typename OutputType,
    typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct cos : public ApplyUnaryFunctionBase<cos, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return InputTypeHelper::cos(x); }
  static OutputType grad_detail(const InputType& x) { return -InputTypeHelper::sin(x); }
};

template <
    typename InputType,
    typename OutputType,
    typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct exp : public ApplyUnaryFunctionBase<exp, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return InputTypeHelper::exp(x); }
  static OutputType grad_detail(const InputType& x) { return InputTypeHelper::exp(x); }
};

template <
    typename InputType,
    typename OutputType,
    typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct log : public ApplyUnaryFunctionBase<log, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return InputTypeHelper::log(x); }
  static OutputType grad_detail(const InputType& x) { return 1 / x; }
};

template <
    typename InputType,
    typename OutputType,
    typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct max0 : public ApplyUnaryFunctionBase<max0, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return x > InputTypeHelper::zero ? x : 0; }
  static OutputType grad_detail(const InputType& x) { return x > InputTypeHelper::zero ? 1 : 0; }
};

template <
    typename InputType,
    typename OutputType,
    typename InputTypeHelper=type_helper::TypeHelper<InputType>
>
struct min0 : public ApplyUnaryFunctionBase<min0, InputType, OutputType> {
  static OutputType apply_detail(const InputType& x) { return x < InputTypeHelper::zero ? x : 0; }
  static OutputType grad_detail(const InputType& x) { return x < InputTypeHelper::zero ? 1 : 0; }
};

//}}}
} // namespace basics

// {{{ DEFINE_UNARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE)
#define DEFINE_UNARY_OPERATORS(OPERATOR_NAME, OPERATOR_TYPE) template< \
  template Operand \
> \
expressions::UnaryExpressions<OPERATOR_TYPE, Operand> OPERATOR_NAME(const Operand& operand) \
{ \
  return expressions::BinaryExpressions<OPERATOR_TYPE, Operand>(operand); \
} \

//}}}

DEFINE_UNARY_OPERATORS(operator-, basics::nagate)
DEFINE_UNARY_OPERATORS(sin, basics::sin)
DEFINE_UNARY_OPERATORS(cos, basics::cos)
DEFINE_UNARY_OPERATORS(exp, basics::exp)
DEFINE_UNARY_OPERATORS(log, basics::log)
DEFINE_UNARY_OPERATORS(max0, basics::max0)
DEFINE_UNARY_OPERATORS(min0, basics::min0)


}
}
}

#endif
