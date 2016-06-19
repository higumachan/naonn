################################################################################
# AutomaticDifferentiation
#  this module generates "libAutomaticDifferentiation.so"
################################################################################

# add library
file(GLOB AUTOMATIC_DIFFERENTIATION_SOURCES src/AutomaticDifferentiation/*.cpp include/AutomaticDifferentiation/*.hpp)
add_library(AutomaticDifferentiation ${AUTOMATIC_DIFFERENTIATION_SOURCES})

# set include directories
target_include_directories(AutomaticDifferentiation PUBLIC include)
target_include_directories(AutomaticDifferentiation PRIVATE include/AutomaticDifferentiation)

