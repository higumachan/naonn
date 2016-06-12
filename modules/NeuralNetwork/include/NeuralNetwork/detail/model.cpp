//
// Created by Yuta Hinokuma on 2016/06/12.
//

#include <model.hpp>

namespace nyao
{
namespace ml
{
namespace neuralnetwork
{

template<
    typename InputLayer,
    typename HiddenLayers,
    typename OutputLayer,
    typename Optimizer
>
traits::output_type Model<
  InputLayer,
  HiddenLayers,
  OutputLayer,
  Optimizer
>::predict(const traits::input_type& input) const
{

}

} // namespace neural network
} // namespace ml
} // namespace nyao
