cmake_minimum_required(VERSION 3.29)
project(ImEngine-Editor)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if (HEADER_FILES OR SOURCE_FILES)
    add_library(ImEngine-Editor SHARED ${HEADER_FILES} ${SOURCE_FILES})

    target_compile_definitions(ImEngine-Editor PRIVATE IMENGINE_EDITOR_EXPORTS)

    target_precompile_headers(ImEngine-Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

    target_include_directories(ImEngine-Editor  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/include)
    target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui)
    target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends)
    target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/deps)

    target_link_libraries(ImEngine-Editor PUBLIC ImEngine-ThirdParty)
else ()
    message(STATUS "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()