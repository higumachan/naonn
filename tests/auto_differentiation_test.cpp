//
// Created by Yuta Hinokuma on 2016/06/14.
//

#include <AutomaticDifferentiation/Variable.hpp>
#include <gtest/gtest.h>

TEST(basic_grad, Variable)
{
  NYAO_NN_VARIABLE(float) t(10);

  ASSERT_FLOAT_EQ(t.get_value(), 10);
  ASSERT_FLOAT_EQ(t.get_grad(t), 1);
}
