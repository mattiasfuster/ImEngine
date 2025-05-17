cmake_minimum_required(VERSION 3.29)
project(ImEngine-Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if (HEADER_FILES OR SOURCE_FILES)
    add_library(ImEngine-Core SHARED ${HEADER_FILES} ${SOURCE_FILES})
    target_compile_definitions(ImEngine-Core PRIVATE IMENGINE_CORE_EXPORTS)

    target_precompile_headers(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

    target_include_directories(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/include)
    target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui)
    target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends)
    target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/deps)

target_link_libraries(ImEngine-Core PUBLIC ImEngine-ThirdParty)
else ()
    message(STATUS "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()