################################################################################
# NeuralNetwork
#  this module generates "libNeuralNetwork.so"
################################################################################

# add library
file(GLOB NEURAL_NETWORK_SOURCES src/NeuralNetwork/*.cpp include/NeuralNetwork/*.hpp)
add_library(NeuralNetwork ${NEURAL_NETWORK_SOURCES})

# set include directories
target_include_directories(NeuralNetwork PUBLIC include)
target_include_directories(NeuralNetwork PRIVATE include/NeuralNetwork)

