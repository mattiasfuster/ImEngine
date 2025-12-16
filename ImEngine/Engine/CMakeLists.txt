cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Engine-----]")

# --- Collect source files automatically ---
file(GLOB_RECURSE ENGINE_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE ENGINE_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# --- Create the Engine as a shared library (DLL) ---
add_library(ImEngine-Engine SHARED
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
)


include(GenerateExportHeader)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h")
generate_export_header(ImEngine-Engine
        EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        EXPORT_MACRO_NAME IMENGINE_API
)
endif()

# --- Precompiled headers (kept private for better incremental build) ----------
target_precompile_headers(ImEngine-Engine
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/misc/EngineExport.h"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/ImEngine-Engine.pch"
)

# --- Public includes ---
target_include_directories(ImEngine-Engine
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/misc"
)

# --- Link third-party dependencies --------------------------------------------
target_link_libraries(ImEngine-Engine
        PUBLIC ImEngine-ThirdParty::glfw
        PUBLIC ImEngine-ThirdParty::vulkan
)

# --- Export/import macro for Windows DLL --------------------------------------
target_compile_definitions(ImEngine-Engine
        PRIVATE IMENGINE_ENGINE_EXPORTS
)

# --- Installation rules (SDK-like) --------------------------------------------
install(TARGETS ImEngine-Engine
        RUNTIME DESTINATION bin          # .dll on Windows
        LIBRARY DESTINATION bin          # .so/.dylib
        ARCHIVE DESTINATION lib          # .lib import/static
        INCLUDES DESTINATION include
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

# --- Modern CMake alias for consumers -----------------------------------------
add_library(ImEngine::Engine ALIAS ImEngine-Engine)
