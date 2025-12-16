cmake_minimum_required(VERSION 3.29)

file(GLOB_RECURSE EDITOR_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE EDITOR_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(ImEngine-Editor
        ${EDITOR_HEADERS}
        ${EDITOR_SOURCES}
)

target_include_directories(ImEngine-Editor
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(ImEngine-Editor
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/misc/EditorExport.h
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/pch.hpp
)

target_link_libraries(ImEngine-Editor
        PRIVATE
        ImEngine-Engine
        ImEngine-ThirdParty::imgui
        ImEngine-ThirdParty::glfw
        ImEngine-ThirdParty::vulkan
)

add_custom_command(TARGET ImEngine-Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:ImEngine-Editor>
        $<TARGET_FILE_DIR:ImEngine-Editor>
        COMMAND_EXPAND_LISTS
)
