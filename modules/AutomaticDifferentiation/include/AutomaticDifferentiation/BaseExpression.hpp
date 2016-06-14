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

template<
    typename Derived,
    typename ValueType,
    typename GradType
>
class BaseExpression
{
  Derived& derived()
  { return static_cast<Derived&>(*this); }

  const Derived& derived() const
  { return static_cast<Derived&>(*this); }

  using value_type = ValueType;
  using grad_type = GradType;

  value_type get_value()
  {
    return derived().get_value();
  }

  grad_type get_grad(const Derived& target)
  {
    return derived().get_grad(target);
  }

  template<typename TargetType>
  grad_type get_grad(const TargetType& target)
  {
    return derived().get_grad(target);
  };

};

}
}
}
}

#endif //NAONN_BASEEXPRESSION_HPP
