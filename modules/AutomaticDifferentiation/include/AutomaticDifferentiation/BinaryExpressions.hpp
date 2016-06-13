#ifndef NAONN_BINARY_OPERATORS_HPP
#define NAONN_BINARY_OPERATORS_HPP

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
{
  Left left;
  Right right;

public:
  BinaryExpression(const Left& l, const Right& r)
      : left(l), right(r)
  { }

  Operator::result_of_apply get_value() const
  {
    return Operator::apply(left.get_value(), right.get_value());
  }

  template<int ID>
  Operator::result_of_grad get_grad(const variable::Variable<float, ID>& target) const
  {
    return Operator::grad(target, left, right);
  }
};

}
}
}
}

#endif
