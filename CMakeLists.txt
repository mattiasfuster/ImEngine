# CMakeGlobal :
cmake_minimum_required(VERSION 3.29)
project(ImEngine)

add_executable(ImEngine ImEngine/main.cpp)

add_subdirectory(ImEngine/Engine)
add_dependencies(ImEngine ImEngine-Engine)

target_link_libraries(ImEngine PRIVATE ImEngine-Engine)

add_custom_command(TARGET ImEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>)