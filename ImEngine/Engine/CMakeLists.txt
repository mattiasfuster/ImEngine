cmake_minimum_required(VERSION 3.29)
project(ImEngine-Engine)

file(GLOB_RECURSE engine_sources *.h *.cpp)
add_library(ImEngine-Engine SHARED ${engine_sources})

target_compile_definitions(ImEngine-Engine PRIVATE IMENGINE_ENGINE_EXPORTS)

target_include_directories(ImEngine-Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)

install(TARGETS ImEngine-Engine RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})