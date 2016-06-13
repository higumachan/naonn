#ifndef NAONN_UNARY_OPERATORS_HPP
#define NAONN_UNARY_OPERATORS_HPP

#include <type_traits>
#include "Variable.hpp"

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
  static OutputType grad(const Variable<TargetType, ID> &target, const InputType &operand) {
    return Derived::grad_detail(operand.get_value()) * operand.get_grad(target);
  }
  using result_of_apply = std::result_of<decltype(apply)>::type;
  using result_of_grad = std::result_of<decltype(grad)>::type;

  static OutputType grad_detail(const InputType& x);
  static OutputType apply_detail(const InputType& x);
};

//}}}

//{{{ basic unary operators
template <typename Type>
struct sin : public ApplyUnaryFunctionBase<sin, Type> {
  static Type apply_detail(const Type& x) { return std::sin(x); }
  static Type grad_detail(const Type& x) { return std::cos(x); }
};

template <typename Type>
struct cos : public ApplyUnaryFunctionBase<cos, Type> {
  static Type apply_detail(const Type& x) { return std::cos(x); }
  static Type grad_detail(const Type& x) { return -std::sin(x); }
};

template <typename Type>
struct exp : public ApplyUnaryFunctionBase<exp, Type> {
  static Type apply_detail(const Type& x) { return std::exp(x); }
  static Type grad_detail(const Type& x) { return std::exp(x); }
};
}
//}}}

}
}
}

#endif
