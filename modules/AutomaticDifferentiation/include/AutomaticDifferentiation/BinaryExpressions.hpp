#ifndef NAONN_BINARY_EXPRESSIONS_HPP
#define NAONN_BINARY_EXPRESSIONS_HPP

#include "Expression.hpp"
#include "Variable.hpp"
#include <iostream>

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace expressions
{

namespace impl
{
template<
    typename Left,
    typename Operator,
    typename Right
>
class BinaryExpression
{
  const Left left;
  const Right right;

public:
  BinaryExpression(const Left& l, const Right& r)
      : left(l), right(r)
  { }

  using value_type = typename Operator::result_of_apply;
  using grad_type = typename Operator::result_of_grad;

  value_type get_value() const
  {
    return Operator::apply(left, right);
  }

  template<typename TargetType, int ID>
  grad_type get_grad(const variable::Variable <TargetType, ID>& target) const
  {
    return Operator::grad(target, left, right);
  }
};
} // impl

template<
    typename Left,
    typename Operator,
    typename Right
>
using BinaryExpression = Expression<impl::BinaryExpression<Left, Operator, Right>>;

}
}
}
}

#endif
