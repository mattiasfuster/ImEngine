cmake_minimum_required(VERSION 3.29)
project(ImEngine-Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
        include/VulkanContext.h
)

set(SOURCE_FILES
        src/VulkanContext.cpp
)

add_library(ImEngine-Core SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(ImEngine-Core PRIVATE IMENGINE_CORE_EXPORTS)

target_precompile_headers(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

target_include_directories(ImEngine-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/include)
target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui)
target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui/backends)
target_include_directories(ImEngine-Core PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/glfw/deps)

target_link_libraries(ImEngine-Core PUBLIC ImEngine-ThirdParty)

install(TARGETS ImEngine-Core
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib/static
)