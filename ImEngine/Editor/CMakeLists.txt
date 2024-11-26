cmake_minimum_required(VERSION 3.29)
project(ImEngine-Editor)

file(GLOB_RECURSE editor_sources *.h *.cpp)
add_library(ImEngine-Editor SHARED ${editor_sources})
target_compile_definitions(ImEngine-Editor PRIVATE IMENGINE_DLL_EXPORTS)
target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)