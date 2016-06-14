//
// Created by Yuta Hinokuma on 2016/06/14.
//

#ifndef NAONN_BASEEXPRESSION_HPP
#define NAONN_BASEEXPRESSION_HPP

#include "Variable.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace expressions
{

template<typename Derived>
class BaseExpression
{
  Derived& derived()
  { return static_cast<Derived&>(*this); }

  const Derived& derived() const
  { return static_cast<Derived&>(*this); }

  template <typename ValueType>
  ValueType get_value()
  {
    return derived().get_value();
  }

  template <typename GradType>
  GradType get_grad(const Derived& target)
  {
    return derived().get_grad(target);
  }

  template <typename TargetType, typename GradType>
  GradType get_grad(const TargetType& target)
  {
    return derived().get_grad(target);
  };

};

}
}
}
}

#endif //NAONN_BASEEXPRESSION_HPP
