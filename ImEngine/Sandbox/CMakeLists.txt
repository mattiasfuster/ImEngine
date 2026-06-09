cmake_minimum_required(VERSION 4.3.3)
message(STATUS "[-----ImEngine - Sandbox-----]")

add_executable(ImEngine_Sandbox src/main.cpp)
target_link_libraries(ImEngine_Sandbox PRIVATE ImEngine_Core)
