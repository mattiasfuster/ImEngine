cmake_minimum_required(VERSION 4.3.3)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
project(ImEngine
    DESCRIPTION "Modern C++23 game engine"
    HOMEPAGE_URL "https://github.com/mattiasfuster/ImEngine"
    LANGUAGES C CXX
)

message(STATUS "[-----ImEngine-----]")

add_subdirectory(ImEngine/Core)
add_subdirectory(ImEngine/Sandbox)