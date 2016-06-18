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
namespace type_helpe
{

template<typename Type>
struct TypeHelper
{
  using type = Type;

};

template<>
struct TypeHelper<float>
{
  using type = float;

  static float zero()
  {
    return 0.0f;
  }

  static float one()
  {
    return 1.0f;
  }

  static float negate(float x)
  {
    return -x;
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

  template<typename T>
  static float plus(float x, const T& y)
  {
    return x + y;
  }

  template<typename T>
  static float minus(float x, const T& y)
  {
    return x - y;
  }

  template<typename T>
  static float multiply(float x, const T& y)
  {
    return x * y;
  }

  template <typename T>
  static float divide(float x, const T& y)
  {
    return x / y;
  }

  template <typename T>
  static float max(float x, const T& y)
  {
    return x > y ? x : static_cast<float>(y);
  }

  template <typename T>
  static float min(float x, const T& y)
  {
    return x < y ? x : static_cast<float>(y);
  }
};

} // type_helper
} // automatic
} // differentiation
} // nyao

#endif //NAONN_VARIABLETYPEWRAPPER_HPP
