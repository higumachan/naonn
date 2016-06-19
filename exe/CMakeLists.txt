################################################################################
# Add executables
################################################################################

message(STATUS "Added the following executables:")
file(GLOB EXE_SOURCES *.cpp)
foreach(EXE_SOURCE ${EXE_SOURCES})
    get_filename_component(EXE_NAME ${EXE_SOURCE} NAME_WE)
    add_executable(${EXE_NAME} ${EXE_SOURCE})
    target_link_libraries(${EXE_NAME} detector ${DLIB_LIBRARIES})
    message(STATUS "  ${EXE_NAME}")
endforeach()
