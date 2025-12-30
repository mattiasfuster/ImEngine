cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Engine-----]")

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

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Engine)
enable_security_flags(ImEngine-Engine)

include(GenerateExportHeader)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h")
generate_export_header(ImEngine-Engine
        EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        EXPORT_MACRO_NAME IMENGINE_ENGINE_API
)
endif()

# --- Precompiled headers ---
target_precompile_headers(ImEngine-Engine
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/ImEngine-Engine.pch"
)

# --- Public includes ---
target_include_directories(ImEngine-Engine
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>
            $<INSTALL_INTERFACE:include/ImEngine>
)

# --- Link dependencies ---
target_link_libraries(ImEngine-Engine
        PUBLIC ImEngine::Core
        PUBLIC ImEngine-ThirdParty::glfw
        PUBLIC ImEngine-ThirdParty::vulkan
)

target_compile_definitions(ImEngine-Engine
        PRIVATE IMENGINE_ENGINE_EXPORTS
)

# --- Installation rules ---
install(TARGETS ImEngine-Engine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        DESTINATION "include/ImEngine")

add_library(ImEngine::Engine ALIAS ImEngine-Engine)
