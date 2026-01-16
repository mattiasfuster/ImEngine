cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Engine-----]")

# 1. SHADER COMPILATION

find_package(Vulkan REQUIRED)

if (NOT Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found! Please check your Vulkan SDK installation.")
endif()

# Recursively find all shader files
# CONFIGURE_DEPENDS ensures CMake re-runs if a new shader file is added
file(GLOB_RECURSE SHADER_SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.comp"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.geom"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.tesc"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.tese"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.mesh"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.task"
)

set(SPV_BINARY_FILES "")

foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
    # Extract filename
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME)
    
    # Define output path in the temporary build directory
    set(SPV_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_NAME}.spv")

    # Ensure output directory exists
    get_filename_component(SPV_DIR ${SPV_OUTPUT} DIRECTORY)
    file(MAKE_DIRECTORY ${SPV_DIR})

    # Command to compile GLSL to SPIR-V
    add_custom_command(
        OUTPUT ${SPV_OUTPUT}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER_SOURCE} -o ${SPV_OUTPUT}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling shader: ${SHADER_NAME} -> ${SHADER_NAME}.spv"
    )

    list(APPEND SPV_BINARY_FILES ${SPV_OUTPUT})
endforeach()

# Custom target to group all shader compilation tasks
add_custom_target(ImEngine_Engine_Shaders ALL DEPENDS ${SPV_BINARY_FILES})


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

# --- Link the Shaders to the Library ---
# CRITICAL: Ensure shaders are compiled before the library is linked/built
add_dependencies(ImEngine-Engine ImEngine_Engine_Shaders)

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

# Install the compiled SPIR-V shaders next to the binary
install(FILES ${SPV_BINARY_FILES}
        DESTINATION "bin/shaders"
)

# Create an alias for easier linking by other targets
add_library(ImEngine::Engine ALIAS ImEngine-Engine)