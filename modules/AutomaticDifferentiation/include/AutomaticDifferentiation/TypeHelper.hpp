//
// Created by Yuta Hinokuma on 2016/06/13.
//

#ifndef NAONN_VARIABLETYPEWRAPPER_HPP
#define NAONN_VARIABLETYPEWRAPPER_HPP

#include <cmath>
#include <string>
#include <sstream>

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

template<>
struct TypeHelper<std::string>
{
  using type = std::string;

  static type zero()
  {
    return "0";
  }

  static type one()
  {
    return "1";
  }

  static type negate(const type& x)
  {
    if (x[0] != '-'){
      return type("-") + x;
    }
    return x.substr(1, x.size() - 1);
  }

  static type sin(const type& x)
  {
    return type("sin(") + x + type(")");
  }

  static type cos(const type& x)
  {
    return type("cos(") + x + type(")");
  }

  static type exp(const type& x)
  {
    return type("exp(") + x + type(")");
  }

  static type log(const type& x)
  {
    return type("log(") + x + type(")");
  }

  template<typename T>
  static type plus(type x, const T& y)
  {
    if (y == TypeHelper<T>::zero()){
      return x;
    }
    if (x == zero()){
      return to_s(y);
    }
    return x + "+" + to_s(y);
  }

  template<typename T>
  static type plus(const T& y, const type& x, typename std::enable_if<!std::is_same<T, type>::value>::type* = nullptr)
  {
    if (y == TypeHelper<T>::zero()){
      return x;
    }
    if (x == zero()){
      return to_s(y);
    }
    return to_s(y) + "+" + x;
  }


  template<typename T>
  static type minus(type x, const T& y)
  {
    if (y == TypeHelper<T>::zero()){
      return x;
    }
    if (x == zero()){
      return to_s(y);
    }
    return x + "-" + to_s(y);
  }

  template<typename T>
  static type multiply(type x, const T& y)
  {
    if (y == TypeHelper<T>::zero() || x == zero()){
      return zero();
    }
    if (y == TypeHelper<T>::one()){
      return x;
    }
    if (x == one()){
      return to_s(y);
    }
    return x + "*" + to_s(y);
  }

  template<typename T>
  static type multiply(const T& y, const type& x, typename std::enable_if<!std::is_same<T, type>::value>::type* = nullptr)
  {
    if (y == TypeHelper<T>::zero() || x == zero()){
      return zero();
    }
    if (y == TypeHelper<T>::one()){
      return x;
    }
    if (x == one()){
      return to_s(y);
    }
    return to_s(y) + "*" + x;
  }

  template <typename T>
  static type divide(type x, const T& y)
  {
    if (y == TypeHelper<T>::one()){
      return x;
    }
    if (x == one()){
      return to_s(y);
    }
    return x + "/" + to_s(y);
  }

  template <typename T>
  static type max(type x, const T& y)
  {
    return x > y ? x : static_cast<type>(y);
  }

  template <typename T>
  static type min(type x, const T& y)
  {
    return x < y ? x : static_cast<type>(y);
  }

  template <typename T>
  static std::string to_s(const T& t)
  {
    std::ostringstream oss;

    oss << t;

    return oss.str();
  }
};

} // type_helper
} // automatic
} // differentiation
} // nyao

#endif //NAONN_VARIABLETYPEWRAPPER_HPP
