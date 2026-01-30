cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Core-----]")

set(CORE_HEADERS
        include/Core/Maths/Units.h
        include/Core/Maths/Constants.h
        include/Core/Maths/Concepts.h
        include/Core/Maths/Objects/Vector.h
        include/Core/Maths/Objects/Matrice.h
        include/Core/Maths/Objects/Quaternion.h
        include/Core/Debug/Logger.h
)

set(CORE_SOURCES
        src/Core.cpp
)

add_library(ImEngine-Core SHARED
        ${CORE_HEADERS}
        ${CORE_SOURCES}
)

set_project_warnings(ImEngine-Core)
enable_security_flags(ImEngine-Core)

include(GenerateExportHeader)
generate_export_header(ImEngine-Core
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/CoreExport.h"
        EXPORT_MACRO_NAME IMENGINE_CORE_API
)

target_precompile_headers(ImEngine-Core
        PRIVATE
            <cstddef>
            <cstdint>
            <memory>
            <string>
            <vector>
)

target_include_directories(ImEngine-Core
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<INSTALL_INTERFACE:include/ImEngine>
)

target_link_libraries(ImEngine-Core
)

target_compile_definitions(ImEngine-Core
        PRIVATE IMENGINE_CORE_EXPORTS
        PRIVATE $<$<CONFIG:Debug>:IMENGINE_DEBUG>
        PRIVATE $<$<CONFIG:Release>:IMENGINE_RELEASE>
        PRIVATE $<$<CONFIG:RelWithDebInfo>:IMENGINE_RELWITHDEBINFO>
)

install(TARGETS ImEngine-Core
        RUNTIME DESTINATION bin          # .dll on Windows
        LIBRARY DESTINATION bin          # .so/.dylib
        ARCHIVE DESTINATION lib          # .lib import/static
        INCLUDES DESTINATION include
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "include/ImEngine")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CoreExport.h"
        DESTINATION "include/ImEngine")

add_library(ImEngine::Core ALIAS ImEngine-Core)
