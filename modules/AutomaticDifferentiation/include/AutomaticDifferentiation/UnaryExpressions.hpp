//
// Created by Yuta Hinokuma on 2016/06/13.
//

#ifndef NAONN_UNARYEXPRESSIONS_HPP
#define NAONN_UNARYEXPRESSIONS_HPP

#include "Variable.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace expressions
{

template<typename Operator, typename Operand>
class UnaryExpression
{
  Operand operand;

public:
  UnaryExpression(const Operand& _operand)
      : operand(_operand)
  { }

  using value_type = typename Operator::result_of_apply;
  using grad_type = typename Operator::result_of_grad;

  value_type get_value() const
  {
    return Operator::apply(operand.get_value());
  }

  template<typename TargetType, int ID>
  grad_type grad(const variable::Variable<TargetType, ID>& target) const
  {
    return Operator::grad(target, operand);
  }
};


}
}
}
}


#endif //NAONN_UNARYEXPRESSIONS_HPP
