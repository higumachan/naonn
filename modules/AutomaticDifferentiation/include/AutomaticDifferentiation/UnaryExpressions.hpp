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

  Operator::result_of_apply get_value() const
  {
    return Operator::apply(operand.get_value());
  }

  template<int ID>
  Operator::result_of_grad get_grad(const variable::Variable<float, ID>& target) const
  {
    return Operator::grad(target, operand);
  }
};


}
}
}
}


#endif //NAONN_UNARYEXPRESSIONS_HPP
