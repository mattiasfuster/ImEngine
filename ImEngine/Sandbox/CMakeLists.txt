cmake_minimum_required(VERSION 3.29)
project(ImEngine-Sandbox)

if (IMENGINE_BUILD_SANDBOX)
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    if (HEADER_FILES OR SOURCE_FILES)
        add_executable(ImEngine-Sandbox ${HEADER_FILES} ${SOURCE_FILES})

        target_include_directories(ImEngine-Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

        target_link_libraries(ImEngine-Sandbox PRIVATE ImEngine-ThirdParty::glfw ImEngine-ThirdParty::vulkan ImEngine-Engine)
    else ()
        message(STATUS "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endif()