cmake_minimum_required(VERSION 3.29)
project(ImEngine-Engine)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if (HEADER_FILES OR SOURCE_FILES)
    add_library(ImEngine-Engine STATIC ${HEADER_FILES} ${SOURCE_FILES})

    target_compile_definitions(ImEngine-Engine PRIVATE IMENGINE_ENGINE_EXPORTS)

    target_precompile_headers(ImEngine-Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

    target_include_directories(ImEngine-Engine
            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(ImEngine-Engine
            PUBLIC ImEngine-ThirdParty
    )
else ()
    message(STATUS "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()