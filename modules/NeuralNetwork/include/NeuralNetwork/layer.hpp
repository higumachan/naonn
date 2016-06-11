//
// Created by Yuta Hinokuma on 2016/06/12.
//

#ifndef NAONN_LAYER_HPP
#define NAONN_LAYER_HPP

template<
    typename Derived
>
class LayerBase
{
  Derived& derived() {
    return static_cast<Derived&>(*this);
  }
  const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }
};

#endif //NAONN_LAYER_HPP
