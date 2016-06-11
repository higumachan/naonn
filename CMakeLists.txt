cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(naonn CXX)


################################################################################
# Forbid in-source building
################################################################################

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()


################################################################################
# Set build types and cxx flags
################################################################################

# show compiler
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

# build type (default is RelWithDebInfo)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo." FORCE)
endif()

if(NOT(CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo OR CMAKE_BUILD_TYPE STREQUAL Release))
    message(FATAL_ERROR "Choose the type of build, options are: Debug RelWithDebInfo Release.")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# add cxx flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")        # enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")     # enable all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare") # disable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits")  # disable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")  # force pedantic
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")     # enable all SSE and AVX

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")  # supress warning on gcc
endif()

# add some options to CMAKE_CXX_FLAGS_DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -DTBB_DO_ASSERT=1")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_LIBCPP_DEBUG=0")
endif()

# override CMAKE_CXX_FLAGS_RELWITHDEBINFO
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Og")
else()
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2")
endif()

# add definitions
add_definitions(-D_USE_MATH_DEFINES) # enable math defines (M_PI)

option(ENABLE_STOPWATCH "Enable StopWatch" OFF)
if(ENABLE_STOPWATCH)
    add_definitions(-DENABLE_STOPWATCH)    # enable util::StopWatch
endif()

# print cxx flags
string(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" CMAKE_CXX_FLAGS_BUILD_TYPE)
set(CMAKE_CXX_FLAGS_BUILD_TYPE "${${CMAKE_CXX_FLAGS_BUILD_TYPE}}")
message(STATUS "CXX flags:${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BUILD_TYPE}")


################################################################################
# Check libraries
################################################################################

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add cmake directory to CMake's module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# remove 'lib' prefix for shared libraries on Windows
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Eigen
set(Eigen_INCLUDE_DIR "/usr/local/include/eigen3")
include_directories(${Eigen_INCLUDE_DIR})


################################################################################
# Check compile support tools
################################################################################

# TC Malloc (gpreftools)
option(USE_TC_MALLOC "Use TC Malloc" OFF)

if(USE_TC_MALLOC)
    find_library(TC_Malloc_LIBRARY tcmalloc)
    if(TC_Malloc_LIBRARY STREQUAL TC_Malloc_LIBRARY-NOTFOUND)
        message(STATUS "TC Malloc is not found")
    else()
        message(STATUS "Found TC Malloc: ${TC_Malloc_LIBRARY}")
        link_libraries(${TC_Malloc_LIBRARY})
    endif()
endif()

# CPU profiler (gpreftools)
option(USE_CPU_PROFILER "Use CPU profiler" OFF)

if(USE_CPU_PROFILER)
    find_library(CPU_PROFILER_LIBRARY profiler)
    if(CPU_PROFILER_LIBRARY STREQUAL CPU_PROFILER_LIBRARY-NOTFOUND)
        message(STATUS "CPU profiler is not found")
    else()
        message(STATUS "Found CPU profiler: ${CPU_PROFILER_LIBRARY}")
        link_libraries(${CPU_PROFILER_LIBRARY} "-Wl,-pie")
    endif()
endif()

# ccache
option(USE_CCACHE "Use ccache" OFF)

if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        message(STATUS "ccache is enabled")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    else()
        message(STATUS "ccache is not enabled")
    endif()
endif()


################################################################################
# Add subprojects
################################################################################

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for executables")
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for libraries")

# Add modules
add_subdirectory(modules)

# Add executables
add_subdirectory(exe)

