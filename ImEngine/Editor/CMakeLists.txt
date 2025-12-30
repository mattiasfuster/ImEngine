cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Editor-----]")

set(EDITOR_HEADERS
)

set(EDITOR_SOURCES
    src/ImEngine-Editor.cpp
)

add_executable(ImEngine-Editor
        ${EDITOR_HEADERS}
        ${EDITOR_SOURCES}
)

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Editor)
enable_security_flags(ImEngine-Editor)

target_include_directories(ImEngine-Editor
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(ImEngine-Editor
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/ImEngine-Editor.pch.h"
)

target_link_libraries(ImEngine-Editor
        PRIVATE ImEngine::Engine
        PRIVATE ImEngine-ThirdParty::imgui
        # Note: ImEngine::Core, glfw, vulkan are transitively available via Engine
)

add_custom_command(TARGET ImEngine-Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:ImEngine-Editor>
        $<TARGET_FILE_DIR:ImEngine-Editor>
        COMMAND_EXPAND_LISTS
)

install(TARGETS ImEngine-Editor
    RUNTIME DESTINATION bin
)
