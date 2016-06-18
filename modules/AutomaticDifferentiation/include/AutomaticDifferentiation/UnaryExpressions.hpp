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

namespace impl
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

  template<typename TargetType, variable::id_type ID>
  grad_type get_grad(const variable::Variable <TargetType, ID>& target) const
  {
    return Operator::grad(target, operand);
  }
};
} // impl

template<typename Operator, typename Operand>
using UnaryExpression = Expression<impl::UnaryExpression<Operator, Operand>>;

} // expressions
} // automatic
} // differentiation
} // nyao

#endif //NAONN_UNARYEXPRESSIONS_HPP
