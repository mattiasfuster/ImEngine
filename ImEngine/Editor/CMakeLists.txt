cmake_minimum_required(VERSION 3.29)
project(ImEngine-Editor)

set(HEADER_FILES
)

set(SOURCE_FILES
)

add_library(ImEngine-Editor SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_compile_definitions(ImEngine-Editor PRIVATE IMENGINE_DLL_EXPORTS)
target_include_directories(ImEngine-Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)