//
// Created by Yuta Hinokuma on 2016/06/14.
//

#include <AutomaticDifferentiation/Variable.hpp>
#include <AutomaticDifferentiation/BinaryOperators.hpp>
#include <type_traits>
#include <gtest/gtest.h>

TEST(basic_grad, Variable)
{
  NYAO_NN_VARIABLE(float) t(10);

  ASSERT_FLOAT_EQ(t.get_value(), 10);
  ASSERT_FLOAT_EQ(t.get_grad(t), 1);
}

using namespace nyao::differentiation::automatic;
using namespace nyao::differentiation::automatic::operators;

namespace {
template<
  typename Left,
  typename Type
>
using RightConstBinaryExpression = expressions::BinaryExpression<
    expressions::Expression<Left>,
    ApplyBinaryOperator<
        basics::plus,
        Left,
        variable::ConstVariable<Type>
    >,
    variable::ConstVariable<Type>
>;

template <
    typename Left,
    typename Right
>
using ApplyedBinaryExpression = expressions::BinaryExpression<
    expressions::Expression<Left>,
    ApplyBinaryOperator<
        basics::plus,
        Left,
        Right
    >,
    expressions::Expression<Right>
>;
}

template<typename T>
using enable_if_base_of_expression = typename std::enable_if<std::is_base_of<T, expressions::Expression<T>>::value>::type;
template<typename T>
using disable_if_base_of_expression = typename std::enable_if<!std::is_base_of<T, expressions::Expression<T>>::value>::type;

template<
  typename Left,
  typename Type
>
RightConstBinaryExpression<Left, Type> func(
  const expressions::Expression<Left>& left,
  const Type& right
)
{
  return RightConstBinaryExpression<Left, Type>(left, variable::ConstVariable<Type>(right));
}

template<
  typename Left,
  typename Right
>
ApplyedBinaryExpression<Left, Right> func(
    const expressions::Expression<Left>& left,
    const expressions::Expression<Right>& right
  )
{
  return ApplyedBinaryExpression<Left, Right>(left, right);
}

TEST(basic_grad, Plus)
{
   NYAO_NN_VARIABLE(float) t(10.0f);

  std::cout << t.get_value() << std::endl;

 auto t2 = t + 1.0f;
  auto tl = t + t;
  auto t4 = t + t;

  ASSERT_FLOAT_EQ(t.get_value(), 10);
  ASSERT_FLOAT_EQ(t2.get_value(), 11);
  ASSERT_FLOAT_EQ(t2.get_grad(t), 1);
//  ASSERT_FLOAT_EQ(t3.get_value(), 12);
//  ASSERT_FLOAT_EQ(t3.get_grad(t), 1);
  ASSERT_FLOAT_EQ(tl.get_value(), 20);
  ASSERT_FLOAT_EQ(tl.get_grad(t), 2);
  ASSERT_FLOAT_EQ(t4.get_value(), 20);
  ASSERT_FLOAT_EQ(t4.get_grad(t), 2);

}

