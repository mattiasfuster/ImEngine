cmake_minimum_required(VERSION 3.29)
project(ImEngine)

# Main executable
add_executable(ImEngine ImEngine/main.cpp)

# Always include ThirdParty
add_subdirectory(ImEngine/ThirdParty)
target_link_libraries(ImEngine PUBLIC ImEngine-ThirdParty)

# Optionally add Engine
option(IMENGINE_BUILD_ENGINE "Build the Engine module" ON)
if(IMENGINE_BUILD_ENGINE)
    target_compile_definitions(ImEngine PUBLIC IMENGINE_ENGINE_BUILD)

    add_subdirectory(ImEngine/Engine)
    target_link_libraries(ImEngine PUBLIC ImEngine-Engine)

    # Optionally add Editor
    option(IMENGINE_BUILD_EDITOR "Build the Editor module" OFF)
    if(IMENGINE_BUILD_EDITOR)
        add_subdirectory(ImEngine/Editor)
        target_link_libraries(ImEngine PUBLIC ImEngine-Editor)
    endif()
endif()

add_custom_command(TARGET ImEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
)