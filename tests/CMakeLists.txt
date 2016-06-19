################################################################################
# Add test
################################################################################

set(${TEST_RUN_SOURCE} run_tests.cpp)

message(STATUS "Added the following tests:")
file(GLOB TEST_SOURCES *.cpp)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(STATUS "  ${TEST_NAME}")
endforeach()

add_executable(run_tests ${TEST_RUN_SOURCE} ${TEST_SOURCES})
target_link_libraries(run_tests gtest AutomaticDifferentiation NeuralNetwork)

