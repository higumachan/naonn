#ifndef NAONN_BINARY_OPERATORS_HPP
#define NAONN_BINARY_OPERATORS_HPP

#include "BaseExpression.hpp"
#include "Variable.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace expressions
{

template<class Left, class Operator, class Right>
class BinaryExpression
    : public BaseExpression<BinaryExpression<Left, Operator, Right>>
{
  Left left;
  Right right;

public:
  BinaryExpression(const Left& l, const Right& r)
      : left(l), right(r)
  { }

  using value_type = typename Operator::result_of_apply;
  using grad_type = typename Operator::result_of_apply;

  value_type get_value() const
  {
    return Operator::apply(left.get_value(), right.get_value());
  }

  template<typename TargetType, int ID>
  grad_type get_grad(const variable::Variable<TargetType, ID>& target) const
  {
    return Operator::grad(target, left, right);
  }
};

}
}
}
}

#endif
