//
// Created by Yuta Hinokuma on 2016/06/12.
//

#ifndef NAONN_MODEL_HPP
#define NAONN_MODEL_HPP

#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <random>

namespace nyao
{
namespace ml
{
namespace neuralnetwork
{

namespace internal
{

template <
    typename InputLayer,
    typename HiddenLayers,
    typename OutputLayer
>
struct traits
{
  using input_type = typename InputLayer::value_type;
  using output_type = typename OutputLayer::value_type;
};

} // namespace internal

template<
    typename InputLayer,
    typename HiddenLayers,
    typename OutputLayer,
    typename Optimizer
>
class Model
{
  template <typename T>
  using sequence_type = std::vector<T>;
  using traits = internal::traits<InputLayer, HiddenLayers, OutputLayer>;
  using scalar = float;
  using objective_function_type = std::binary_function<
      const traits::output_type&,
      const traits::output_type&,
      scalar
  >;

public:
  traits::output_type predict(const traits::input_type& input) const
  {
    return output_layer(hidden_layers(input_layer(input)));
  }
  sequence_type<traits::output_type> predict(const sequence_type<traits::input_type>& inputs) const
  {
    sequence_type<traits::output_type> result;

    std::transform(inputs.begin(), inputs.end(), result.begin(), predict);

    return result;
  }
  void fit(
      const sequence_type<traits::input_type>& inputs,
      const sequence_type<traits::output_type>& trains,
      std::size_t epoch,
      std::size_t batch_size=32
  )
  {
    std::default_random_engine rng(std::random_device());

    sequence_type<size_t> indices(inputs.size());
    std::iota(indices.begin(), indices.end(), 0);

    sequence_type<const traits::input_type&> inputs_on_batch(inputs.size());
    sequence_type<const traits::output_type&> trains_on_batch(trains.size());

    for (int i = 0; i < epoch; i++){
      std::shuffle(indices.begin(), indices.end(), rng);
      for (int batch_no = 0; batch_no < inputs.size() / batch_size; batch_no++){
        for (int j = 0; j < batch_size; j++){
          inputs_on_batch[j] = inputs[indices[batch_no * batch_size + j]];
          trains_on_batch[j] = trains[indices[batch_no * batch_size + j]];
        }
        fit_on_batch(inputs_on_batch, trains_on_batch, batch_size);
      }
    }
  }

private:
  void fit_on_batch(
      const sequence_type<const traits::input_type>& inputs,
      const sequence_type<const traits::output_type>& trains,
      std::size_t batch_size
  )
  {
    for (int i = 0; i < batch_size; i++){
      scalar loss = objective_function(predict(inputs[i]), trains[i]);
      update(loss);
    }
  }

  void update(const scalar& loss);

  InputLayer input_layer;
  HiddenLayers hidden_layers;
  OutputLayer output_layer;

  objective_function_type objective_function;

};

} // namespace neural network
} // namespace ml
} // namespace nyao


#endif //NAONN_MODEL_HPP
