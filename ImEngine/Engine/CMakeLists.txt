cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Engine-----]")

# 1. SHADER COMPILATION
add_subdirectory(shaders)


# 2. LIBRARY CONFIGURATION
set(ENGINE_HEADERS
    include/Engine.h
)

set(ENGINE_SOURCES
    src/Engine.cpp
)

# --- Create the Engine as a shared library (DLL) ---
add_library(ImEngine-Engine SHARED
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
)

# --- Ensure shaders are compiled before the library is linked/built
add_dependencies(ImEngine-Engine ImEngine-Shaders)

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Engine)
enable_security_flags(ImEngine-Engine)

include(GenerateExportHeader)
generate_export_header(ImEngine-Engine
        EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        EXPORT_MACRO_NAME IMENGINE_ENGINE_API
)

# --- Precompiled headers ---
target_precompile_headers(ImEngine-Engine
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/ImEngine-Engine.pch.h"
)

# --- Public includes ---
target_include_directories(ImEngine-Engine
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>
            $<INSTALL_INTERFACE:include/ImEngine>
)

# --- Link dependencies ---
# Note: glfw and vulkan are transitively available via ImEngine::Core (PUBLIC link)
target_link_libraries(ImEngine-Engine
        PUBLIC ImEngine::Core
)

target_compile_definitions(ImEngine-Engine
        PRIVATE IMENGINE_ENGINE_EXPORTS
        PRIVATE $<$<CONFIG:Debug>:IMENGINE_DEBUG>
        PRIVATE $<$<CONFIG:Release>:IMENGINE_RELEASE>
        PRIVATE $<$<CONFIG:RelWithDebInfo>:IMENGINE_RELWITHDEBINFO>
)

# 3. INSTALLATION RULES
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        DESTINATION "include/ImEngine")

# Install compiled shaders (from shaders/CMakeLists.txt)
# You may need to add an install rule in shaders/CMakeLists.txt if required

# Create an alias for easier linking by other targets
add_library(ImEngine::Engine ALIAS ImEngine-Engine)