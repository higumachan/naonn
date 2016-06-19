//
// Created by Yuta Hinokuma on 2016/06/14.
//

#include <AutomaticDifferentiation/Variable.hpp>
#include <AutomaticDifferentiation/BinaryOperators.hpp>
#include <AutomaticDifferentiation/UnaryOperators.hpp>
#include <type_traits>
#include <gtest/gtest.h>
#include <cmath>

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

TEST(basic_grad, Multi)
{
   NYAO_NN_VARIABLE(float) t(10.0f);
   NYAO_NN_VARIABLE(float) k(20.0f);

  std::cout << t.get_value() << std::endl;

  auto t2 = t * 2.0f;  //2t -> 2
  auto t3 = t * t; //t^2 -> 2t
  auto t4 = t * k; // (t * k)'t -> k

  ASSERT_FLOAT_EQ(t.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t2.get_value(), 20.0f);
  ASSERT_FLOAT_EQ(t2.get_grad(t), 2.0f);
  ASSERT_FLOAT_EQ(t3.get_value(), 100.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(t), 20.0f);
  ASSERT_FLOAT_EQ(t4.get_value(), 200.0f);
  ASSERT_FLOAT_EQ(t4.get_grad(t), 20.0f);
  ASSERT_FLOAT_EQ(t4.get_grad(k), 10.0f);
}

TEST(basic_grad, Max)
{
   NYAO_NN_VARIABLE(float) t(10.0f);
   NYAO_NN_VARIABLE(float) k(20.0f);

  std::cout << t.get_value() << std::endl;

  auto t2 = operators::max(t, 2.0f);
  auto t3 = operators::max(t, k);
  auto t4 = operators::max(t, 100.0f);

  ASSERT_FLOAT_EQ(t.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t2.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t2.get_grad(t), 1.0f);
  ASSERT_FLOAT_EQ(t3.get_value(), 20.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(t), 0.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(k), 1.0f);
  ASSERT_FLOAT_EQ(t4.get_value(), 100.0f);
  ASSERT_FLOAT_EQ(t4.get_grad(t), 0.0f);
}

TEST(basic_grad, Min)
{
   NYAO_NN_VARIABLE(float) t(10.0f);
   NYAO_NN_VARIABLE(float) k(20.0f);

  std::cout << t.get_value() << std::endl;

  auto t2 = operators::min(t, 2.0f);
  auto t3 = operators::min(t, k);
  auto t4 = operators::min(t, 100.0f);

  ASSERT_FLOAT_EQ(t.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t2.get_value(), 2.0f);
  ASSERT_FLOAT_EQ(t2.get_grad(t), 0.0f);
  ASSERT_FLOAT_EQ(t3.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(t), 1.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(k), 0.0f);
  ASSERT_FLOAT_EQ(t4.get_value(), 10.0f);
  ASSERT_FLOAT_EQ(t4.get_grad(t), 1.0f);
}

TEST(basic_grad, Negate)
{
   NYAO_NN_VARIABLE(float) t(10.0f);
   NYAO_NN_VARIABLE(float) k(-20.0f);

  auto t2 = -t;
  auto t3 = -k;

  ASSERT_FLOAT_EQ(t.get_value(), 10.0f);
  std::cout << t2.get_value() << std::endl;
  ASSERT_FLOAT_EQ(t2.get_value(), -10.0f);
  ASSERT_FLOAT_EQ(t2.get_grad(t), -1.0f);
  ASSERT_FLOAT_EQ(t3.get_value(), 20.0f);
  ASSERT_FLOAT_EQ(t3.get_grad(k), -1.0f);
}

TEST(basic_grad, SinCos)
{
   NYAO_NN_VARIABLE(float) t(M_PI / 2.0f);

  auto t2 = operators::sin(t);
  auto t3 = operators::cos(t);
  float eps = 1e-6;

  ASSERT_FLOAT_EQ(t2.get_value(), 1);
  ASSERT_LT(t2.get_grad(t), eps);
  ASSERT_LT(t3.get_value(), eps);
  ASSERT_FLOAT_EQ(t3.get_grad(t), -1);
}

TEST(basic_grad, Exp)
{
   NYAO_NN_VARIABLE(float) t(1);

  auto t2 = operators::exp(t);
  float eps = 1e-6;

  ASSERT_FLOAT_EQ(t2.get_value(), exp(1));
  ASSERT_FLOAT_EQ(t2.get_grad(t), exp(1));
}

TEST(basic_grad, ComplexMulti)
{
  NYAO_NN_VARIABLE(float) x(2);

  auto t = x * x * x * x;  // x^4 -> 4x^3
  ASSERT_FLOAT_EQ(t.get_value(), 16);
  ASSERT_FLOAT_EQ(t.get_grad(x), 32);

  NYAO_NN_VARIABLE(float) y(3);
  auto t2 = t * exp(y); // x^4 * e^y

  ASSERT_FLOAT_EQ(t2.get_value(), 16 * exp(3)); // == f(x=2, y=3): x^4 * e^y
  ASSERT_FLOAT_EQ(t2.get_grad(x), 32 * exp(3)); // == f(x=2, y=3): (x^4 * e^y)'x = 4x^3 * e^y
  ASSERT_FLOAT_EQ(t2.get_grad(y), 16 * exp(3)); // == f(x=2, y=3): (x^4 * e^y)'y = x^4 * e^y

}

TEST(basic_grad, SetValue)
{
  NYAO_NN_VARIABLE(float) x(2.0f);

  auto t = x * x;

  ASSERT_FLOAT_EQ(t.get_value(), 4.0f);
  ASSERT_FLOAT_EQ(t.get_grad(x), 4.0f);

  x.set_value(4.0f);
  ASSERT_FLOAT_EQ(t.get_value(), 16.0f);
  ASSERT_FLOAT_EQ(t.get_grad(x), 8.0f);
}

TEST(basic_grad, return_variable)
{
  auto func = [](){
    NYAO_NN_VARIABLE(float) x(2.0);

    return x;
  };
  auto x = func();
  auto t = x * x;


  ASSERT_FLOAT_EQ(t.get_value(), 4.0f);
  ASSERT_FLOAT_EQ(t.get_grad(x), 4.0f);
}

TEST(basic_grad, string_variable)
{
  NYAO_NN_VARIABLE(std::string) x("x");

  auto t = x * x;
  auto t2 = operators::sin(x);
  auto t3 = t * t2;  // x^2 * sin(x)
  auto t4 = t * 4.0f;

  ASSERT_EQ(t.get_value(), "x*x");
  ASSERT_EQ(t.get_grad(x), "x+x");
  ASSERT_EQ(t2.get_value(), "sin(x)");
  ASSERT_EQ(t2.get_grad(x), "cos(x)");
  ASSERT_EQ(t3.get_value(), "x*x*sin(x)");
  ASSERT_EQ(t3.get_grad(x), "sin(x)*x+x+x*x*cos(x)");
  ASSERT_EQ(t4.get_value(), "x*x*4");
  ASSERT_EQ(t4.get_grad(x), "4*x+x");
}
