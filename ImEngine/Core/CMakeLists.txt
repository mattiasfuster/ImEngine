cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Core-----]")

set(CORE_HEADERS
    include/Concurrency/FibersSystem/Fiber.h
    include/Memory/MemoryAllocator/MemoryAllocatorBase.h
    include/Memory/MemoryAllocator/Allocators/LinearAllocator.h
    include/Memory/MemoryAllocator/Allocators/MallocAllocator.h
    include/Memory/MemoryAllocator/Allocators/PoolAllocator.h
)

set(CORE_SOURCES
    src/Concurrency/FibersSystem/Fiber.cpp
)

# --- Create the Core as a shared library (DLL) ---
add_library(ImEngine-Core SHARED
        ${CORE_HEADERS}
        ${CORE_SOURCES}
)

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Core)
enable_security_flags(ImEngine-Core)

include(GenerateExportHeader)
generate_export_header(ImEngine-Core
        EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/misc/CoreExport.h"
        EXPORT_MACRO_NAME IMENGINE_API
)

# --- Precompiled headers (kept private for better incremental build) ----------
target_precompile_headers(ImEngine-Core
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/misc/CoreExport.h"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/ImEngine-Core.pch.h"
)

# --- Public includes ---
target_include_directories(ImEngine-Core
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>
            $<INSTALL_INTERFACE:include/ImEngine>
)

# --- Link third-party dependencies --------------------------------------------
target_link_libraries(ImEngine-Core
        PUBLIC ImEngine-ThirdParty::glfw
        PUBLIC ImEngine-ThirdParty::vulkan
        # Add Core dependencies here (e.g. spdlog, fmt)
)
# --- Export/import macro for Windows DLL --------------------------------------
target_compile_definitions(ImEngine-Core
        PRIVATE IMENGINE_CORE_EXPORTS
)

# --- Installation rules (SDK-like) --------------------------------------------
install(TARGETS ImEngine-Core
        RUNTIME DESTINATION bin          # .dll on Windows
        LIBRARY DESTINATION bin          # .so/.dylib
        ARCHIVE DESTINATION lib          # .lib import/static
        INCLUDES DESTINATION include
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/CoreExport.h"
        DESTINATION "include/ImEngine")

# --- Modern CMake alias for consumers -----------------------------------------
add_library(ImEngine::Core ALIAS ImEngine-Core)
