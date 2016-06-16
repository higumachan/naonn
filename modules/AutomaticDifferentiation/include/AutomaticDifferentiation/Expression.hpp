//
// Created by Yuta Hinokuma on 2016/06/14.
//

#ifndef NAONN_BASEEXPRESSION_HPP
#define NAONN_BASEEXPRESSION_HPP

#include "Variable.hpp"
#include <type_traits>

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace expressions
{

template<typename ExpressionImpl>
struct Expression : public ExpressionImpl
{
  template<typename... Args>
  Expression(const Args&... args) : ExpressionImpl(args...) {}

  using value_type = typename ExpressionImpl::value_type;
  using grad_type = typename ExpressionImpl::grad_type;
};

}
}
}
}

#endif //NAONN_BASEEXPRESSION_HPP
