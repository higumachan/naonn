//
// Created by Yuta Hinokuma on 2016/06/13.
//

#ifndef NAONN_VARIABLETYPEWRAPPER_HPP
#define NAONN_VARIABLETYPEWRAPPER_HPP

#include <cmath>

namespace nyao
{
namespace differentiation
{
namespace automatic
{
namespace type_helper
{

template<typename Type>
struct TypeHelper
{

};

template<>
struct TypeHelper<float>
{
  static float zero()
  {
    return 0.0f;
  }

  static float one()
  {
    return 1.0f;
  }

  static float sin(float x)
  {
    return std::sin(x);
  }

  static float cos(float x)
  {
    return std::cos(x);
  }

  static float exp(float x)
  {
    return std::exp(x);
  }

  static float log(float x)
  {
    return std::log(x);
  }

};

}
}
}
}

#endif //NAONN_VARIABLETYPEWRAPPER_HPP
