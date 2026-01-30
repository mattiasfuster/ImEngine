cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Engine-----]")

add_subdirectory(shaders)

set(ENGINE_HEADERS
    include/Engine/Engine.h
    include/Engine/Window/Window.h
    include/Engine/Window/Contexts/GLFWWindow.h
    include/Engine/Rendering/RHI.h
    include/Engine/Rendering/Contexts/VulkanContext.h
)

set(ENGINE_SOURCES
    src/Engine.cpp
    src/Window/Window.cpp
    src/Window/Contexts/GLFWWindow.cpp
    src/Rendering/RHI.cpp
    src/Rendering/Contexts/VulkanContext.cpp
)

add_library(ImEngine-Engine SHARED
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
)

add_dependencies(ImEngine-Engine ImEngine-Shaders)

set_project_warnings(ImEngine-Engine)
enable_security_flags(ImEngine-Engine)

include(GenerateExportHeader)
generate_export_header(ImEngine-Engine
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/EngineExport.h"
        EXPORT_MACRO_NAME IMENGINE_ENGINE_API
)

target_precompile_headers(ImEngine-Engine
        PRIVATE
            <cstddef>
            <cstdint>
            <memory>
            <string>
            <vector>
            <vulkan/vulkan.h>
)

target_include_directories(ImEngine-Engine
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<INSTALL_INTERFACE:include/ImEngine>
)

target_link_libraries(ImEngine-Engine
        PUBLIC ImEngine::Core
        PUBLIC ImEngine-ThirdParty::vulkan   # PUBLIC: Vulkan types exposed in Engine.h
        PRIVATE ImEngine-ThirdParty::glfw    # PRIVATE: GLFW is internal (Window abstraction)
)

target_compile_definitions(ImEngine-Engine
        PRIVATE IMENGINE_ENGINE_EXPORTS
        PRIVATE $<$<CONFIG:Debug>:IMENGINE_DEBUG>
        PRIVATE $<$<CONFIG:Release>:IMENGINE_RELEASE>
        PRIVATE $<$<CONFIG:RelWithDebInfo>:IMENGINE_RELWITHDEBINFO>
)

install(TARGETS ImEngine-Engine
    RUNTIME DESTINATION bin          # .dll on Windows
    LIBRARY DESTINATION bin          # .so/.dylib
    ARCHIVE DESTINATION lib          # .lib import/static
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/EngineExport.h"
        DESTINATION "include/ImEngine")

# Install compiled shaders (from shaders/CMakeLists.txt)
# You may need to add an install rule in shaders/CMakeLists.txt if required

# Create an alias for easier linking by other targets
add_library(ImEngine::Engine ALIAS ImEngine-Engine)