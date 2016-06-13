//
// Created by Yuta Hinokuma on 2016/06/14.
//

#ifndef NAONN_BASEEXPRESSION_HPP
#define NAONN_BASEEXPRESSION_HPP

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

  static Derived::vale_type get_value();

  static Derived::grad_type get_grad(const Derived& target);

  template<typename TargetType, int OTHER_ID>
  static Derived::grad_type get_grad(const variable::Variable <TargetType, ID>& target);

};

}
}
}
}

#endif //NAONN_BASEEXPRESSION_HPP
