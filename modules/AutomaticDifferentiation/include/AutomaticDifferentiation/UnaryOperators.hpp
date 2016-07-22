#ifndef NAONN_UNARY_OPERATORS_HPP
#define NAONN_UNARY_OPERATORS_HPP

#include <type_traits>
#include "Variable.hpp"
#include "UnaryExpressions.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace operators
{

template<
    template<typename>
    class UnaryOperator,
    typename Operand
>
struct ApplyUnaryOperator
{
  using unary_operator = UnaryOperator<
      typename Operand::value_type
  >;
  using result_of_apply = typename unary_operator::result_of_apply;
  using result_of_grad = typename unary_operator::result_of_grad;
  using type_helper = type_helper::TypeHelper<typename Operand::value_type>;

  static result_of_apply apply(const Operand& operand)
  {
    return unary_operator::apply(operand.get_value());
  }


  template<typename TargetType, variable::id_type ID>
  static result_of_grad grad(
      const variable::Variable<TargetType, ID>& target,
      const Operand& operand
  )
  {
    return type_helper::multiply(unary_operator::grad(operand.get_value()), operand.get_grad(target));
  }
};


//}}}

namespace basics
{
//{{{ basic unary operators

template<typename Type>
struct UnaryOperatorBase
{
  using type_helper = type_helper::TypeHelper<Type>;

  template<typename Output>
  static Output apply(const Type&);

  template<typename Output>
  static Output grad(const Type&);

  using result_of_apply = Type;

  using result_of_grad = Type;
};

template<
    typename Type
>
struct negate
    : public UnaryOperatorBase<Type>
{
  using type_helper = type_helper::TypeHelper<Type>;

  static Type apply(const Type& x)
  {
    return -x;
  }

  static Type grad(const Type&)
  {
    return -1;
  }
};

template<
    typename Type
>
struct sin
    : public UnaryOperatorBase<Type>
{
  using type_helper = type_helper::TypeHelper<Type>;

  static Type apply(const Type& x)
  {
    return type_helper::sin(x);
  }

  static Type grad(const Type& x)
  {
    return type_helper::cos(x);
  }

  using result_of_apply = Type;
  using result_of_grad = Type;
};

template<
    typename Type
>
struct cos
    : public UnaryOperatorBase<Type>
{
  using type_helper = type_helper::TypeHelper<Type>;

  static Type apply(const Type& x)
  {
    return type_helper::cos(x);
  }

  static Type grad(const Type& x)
  {
    return type_helper::negate(type_helper::sin(x));
  }
};

template<
    typename Type
>
struct exp
    : public UnaryOperatorBase<Type>
{
  using type_helper = type_helper::TypeHelper<Type>;

  static Type apply(const Type& x)
  { return type_helper::exp(x); }

  static Type grad(const Type& x)
  { return type_helper::exp(x); }
};

template<
    typename Type
>
struct log
    : public UnaryOperatorBase<Type>
{
  using type_helper = type_helper::TypeHelper<Type>;

  static Type apply(const Type& x)
  { return type_helper::log(x); }

  static Type grad(const Type& x)
  { return 1 / x; }
};

//}}}
} // namespace basics

namespace internal
{

template<
    template<typename> class Operator,
    typename Operand
>
using UnaryExpression = expressions::UnaryExpression<
    ApplyUnaryOperator<Operator, Operand>,
    expressions::Expression<Operand>
>;
} // internal

// {{{ NYAO_NN_DEFINE_UNARY_OPERATOR(OPERATOR_NAME, OPERATOR_TYPE)
#define NYAO_NN_DEFINE_UNARY_OPERATOR(OPERATOR_NAME, OPERATOR)\
template< \
  typename Operand \
> \
internal::UnaryExpression<OPERATOR, Operand> OPERATOR_NAME(const expressions::Expression<Operand>& operand) \
{ \
  return internal::UnaryExpression<OPERATOR, Operand>(operand); \
} \

//}}}

NYAO_NN_DEFINE_UNARY_OPERATOR(operator-, basics::negate)

NYAO_NN_DEFINE_UNARY_OPERATOR(sin, basics::sin)

NYAO_NN_DEFINE_UNARY_OPERATOR(cos, basics::cos)

NYAO_NN_DEFINE_UNARY_OPERATOR(exp, basics::exp)

NYAO_NN_DEFINE_UNARY_OPERATOR(log, basics::log)


} // operators
} // automatic
} // differentiation
} // nyao

#endif
