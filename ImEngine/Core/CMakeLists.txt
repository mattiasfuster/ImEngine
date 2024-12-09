cmake_minimum_required(VERSION 3.29)
project(ImEngine-Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
        include/Task.h
)

set(SOURCE_FILES
        src/Task.cpp
)

add_library(ImEngine-Core SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(ImEngine-Core PRIVATE IMENGINE_CORE_EXPORTS)

target_precompile_headers(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

target_include_directories(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ImEngine-Core
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib/static
)