#ifndef NAONN_BINARY_OPERATORS_HPP
#define NAONN_BINARY_OPERATORS_HPP

#include <type_traits>
#include <utility>
#include <sys/wait.h>
#include "Variable.hpp"
#include "BinaryExpressions.hpp"
#include "TypeHelper.hpp"

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace operators
{

template<
    template<typename, typename>
    class BinaryOperator,
    typename Left,
    typename Right
>
struct ApplyBinaryOperator
{
  using binary_operator  = BinaryOperator<
      typename Left::value_type,
      typename Right::value_type
  >;
  using result_of_apply = typename binary_operator::result_of_apply;
  using result_of_grad = typename binary_operator::result_of_grad;

  static result_of_apply apply(const Left& left, const Right& right)
  {
    return binary_operator::apply(left.get_value(), right.get_value());
  }


  template<typename TargetType, int ID>
  static result_of_grad grad(
      const variable::Variable<TargetType, ID>& target,
      const Left& left,
      const Right& right
  )
  {
    return (binary_operator::grad_left(left.get_value(), right.get_value()) *
        left.get_grad(target) +
        binary_operator::grad_right(left.get_value(), right.get_value()) *
            right.get_grad(target)
    );
  }
};

namespace basics
{

template<
    typename InputTypeLeft,
    typename InputTypeRight
>
struct BinaryOperatorBase
{

  template<typename Output>
  static Output apply(const InputTypeLeft& x, const InputTypeRight& y);

  template<typename Output>
  static Output grad(const InputTypeLeft&, const InputTypeRight&);

  template<typename Output>
  static Output grad_left(const InputTypeLeft& left, const InputTypeRight& right)
  {
    return grad<Output>(left, right);
  }

  template<typename Output>
  static Output grad_right(const InputTypeLeft& left, const InputTypeRight& right)
  {
    return grad<Output>(left, right);
  }
};


template<typename InputTypeLeft, typename InputTypeRight>
struct plus
    : public BinaryOperatorBase<InputTypeLeft, InputTypeRight>
{
  using type_helper = type_helper::TypeHelper<InputTypeLeft>;

  //using result_of_apply = decltype(type_helper::plus(std::declval<InputTypeLeft>(), std::declval<InputTypeRight>()));
  using result_of_apply = InputTypeLeft;
  using result_of_grad = typename type_helper::type;

  static result_of_apply apply(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x + y;
  }


  static result_of_grad grad_left(const InputTypeLeft&, const InputTypeRight&)
  { return type_helper::one(); }

  static result_of_grad grad_right(const InputTypeLeft&, const InputTypeRight&)
  { return type_helper::one(); }

};

} // basics


//{{{ ApplyBinaryFunctionBase
#if false
template<
    typename Derived
>
struct ApplyBinaryFunctionBase
{
  Derived& derived()
  { return static_cast<Derived&>(*this); }

  const Derived derived() const
  { return static_cast<Derived&>(*this); }

  /*
  using result_of_apply = typename std::result_of<decltype(Derived::apply_detail)>::type;
  using result_of_grad = typename std::result_of<decltype(Derived::grad_detail)>::type;
   */

  template<typename InputTypeLeft, typename InputTypeRight>
  static typename Derived::output_type apply(InputTypeLeft left, InputTypeRight right)
  {
    return Derived::apply_detail(left.get_value(), right.get_value());
  }

  template<typename Left, typename Right, typename TargetType, typename OutputType, int ID>
  static OutputType grad(
      const variable::Variable<TargetType, ID>& target,
      const Left& left,
      const Right& right
  )
  {
    return (Derived::grad_detail_left(left.get_value(), right.get_value()) *
        left.get_grad(target) +
        Derived::grad_detail_right(left.get_value(), right.get_value()) *
            right.get_grad(target));
  }


  template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
  static OutputType apply_detail(const InputTypeLeft& x, const InputTypeRight& y);

  template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
  static OutputType grad_detail(const InputTypeLeft& x, const InputTypeRight& y);

  template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
  static OutputType grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return Derived::grad_detail(x, y);
  }

  template<typename InputTypeLeft, typename InputTypeRight, typename OutputType>
  static OutputType grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return Derived::grad_detail(x, y);
  }

};
// }}}


namespace basics
{
// {{{ basic operators
template<
    typename InputTypeLeft,
    typename InputTypeRight
>
struct max
    : public ApplyBinaryFunctionBase<
        max<InputTypeLeft, InputTypeRight>
    >
{
  static_assert(std::is_same<InputTypeLeft, InputTypeRight>(), "max operator is require same type left and right");

  using type_helper = type_helper::TypeHelper<InputTypeLeft>;
  using output_type = InputTypeLeft;

  static InputTypeLeft apply_detail(const InputTypeLeft& x, const InputTypeLeft& y)
  {
    return x > y ? x : y;
  }

  static InputTypeLeft grad_detail_left(const InputTypeLeft& x, const InputTypeLeft& y)
  {
    return x > y ? type_helper::one() : type_helper::zero();
  }

  static InputTypeLeft grad_detail_right(const InputTypeLeft& x, const InputTypeLeft& y)
  {
    return x < y ? type_helper::one() : type_helper::zero();
  }

  using result_of_apply = typename std::result_of<decltype(apply_detail)>::type;
  using result_of_grad = typename std::result_of<decltype(grad_detail_left)>::type;
};

template<typename InputTypeLeft, typename InputTypeRight>
struct min
    : public ApplyBinaryFunctionBase<
        min<InputTypeLeft, InputTypeRight>
    >
{
  static_assert(std::is_same<InputTypeLeft, InputTypeRight>(), "min operator is require same type left and right");

  using type_helper = type_helper::TypeHelper<InputTypeLeft>;

  static InputTypeLeft apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x < y ? x : y;
  }

  static InputTypeLeft grad_detail_left(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x < y ? type_helper::one : type_helper::zero;
  }

  static InputTypeLeft grad_detail_right(const InputTypeLeft& x, const InputTypeRight& y)
  {
    return x > y ? type_helper::one : type_helper::zero;
  }

  using result_of_apply = typename std::result_of<decltype(apply_detail)>::type;
  using result_of_grad = typename std::result_of<decltype(grad_detail_left)>::type;
};

template<typename InputTypeLeft, typename InputTypeRight>
struct plus
    : public ApplyBinaryFunctionBase<
        plus<InputTypeLeft, InputTypeRight>
    >
{
  static_assert(std::is_same<InputTypeLeft, InputTypeRight>(), "plus operator is require same type left and right");

  using type_helper = type_helper::TypeHelper<InputTypeLeft>;

  using result_of_apply = decltype(type_helper::multiply( std::declval<InputTypeLeft>(), std::declval<InputTypeRight>()));
  static result_of_apply apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return type_helper::plus(x, y); }

  static typename type_helper::type grad_detail(const InputTypeLeft&, const InputTypeRight&)
  { return type_helper::one(); }

  using result_of_grad = InputTypeLeft;
};

template<typename InputTypeLeft, typename InputTypeRight>
struct multiply
    : public ApplyBinaryFunctionBase<
        multiply<InputTypeLeft, InputTypeRight>
    >
{
  using type_helper = type_helper::TypeHelper<InputTypeLeft>;
  using result_of_apply = decltype(type_helper::multiply( std::declval<InputTypeLeft>(), std::declval<InputTypeRight>()));

  static result_of_apply
  apply_detail(const InputTypeLeft& x, const InputTypeRight& y)
  { return type_helper::multiply(x, y); }

  static InputTypeRight grad_detail_left(const InputTypeLeft&, const InputTypeRight& y)
  { return y; }

  static InputTypeLeft grad_detail_right(const InputTypeLeft& x, const InputTypeRight&)
  { return x; }

  using result_of_grad = InputTypeLeft;
};

// }}}
}
#endif

namespace inner {

template <
    typename Left,
    template<typename, typename> class Operator,
    typename Right
>
using BinaryExpression = expressions::BinaryExpression<
    expressions::Expression<Left>,
    ApplyBinaryOperator<
        Operator,
        Left,
        Right
    >,
    expressions::Expression<Right>
>;

template <
    typename Type,
    template<typename, typename> class Operator,
    typename Right
>
using LeftConstBinaryExpression = expressions::BinaryExpression<
    variable::ConstVariable<Type>,
    ApplyBinaryOperator<
        Operator,
        variable::ConstVariable<Type>,
        Right
    >,
    expressions::Expression<Right>
>;

template <
    typename Left,
    template<typename, typename> class Operator,
    typename Type
>
using RightConstBinaryExpression = expressions::BinaryExpression<
    expressions::Expression<Left>,
    ApplyBinaryOperator<
        Operator,
        Left,
        variable::ConstVariable<Type>
    >,
    variable::ConstVariable<Type>
>;

}

#define NYAONN_DEFINE_BINARY_OPERATORS(OPERATOR_NAME, OPERATOR) \
template< typename Left, typename Right > \
nyao::differentiation::automatic::operators::inner::BinaryExpression<Left, OPERATOR, Right> \
OPERATOR_NAME(const expressions::Expression<Left>& left, const expressions::Expression<Right>& right)\
{ \
  return nyao::differentiation::automatic::operators::inner::BinaryExpression<Left, OPERATOR, Right>(left, right); \
} \
template< typename Left, typename Type > \
nyao::differentiation::automatic::operators::inner::RightConstBinaryExpression<Left, OPERATOR, Type> \
OPERATOR_NAME(const expressions::Expression<Left>& left, const Type& right)\
{\
  return nyao::differentiation::automatic::operators::inner::RightConstBinaryExpression<Left, OPERATOR, Type>( \
    left, \
    variable::ConstVariable<Type>(right) \
  ); \
}\
template< typename Type, typename Right > \
nyao::differentiation::automatic::operators::inner::LeftConstBinaryExpression<Type, OPERATOR, Right> \
OPERATOR_NAME(const Type& left, const expressions::Expression<Right>& right)\
{\
  return nyao::differentiation::automatic::operators::inner::LeftConstBinaryExpression<Type, OPERATOR, Right>(\
    variable::ConstVariable<Type>(left), \
    right \
  ); \
}\


NYAONN_DEFINE_BINARY_OPERATORS(operator+, basics::plus)

/*
DEFINE_BINARY_OPERATORS(operator*, basics::multiply)

DEFINE_BINARY_OPERATORS(min, basics::min)

DEFINE_BINARY_OPERATORS(max, basics::max)
*/


}
}
}
}

#endif
