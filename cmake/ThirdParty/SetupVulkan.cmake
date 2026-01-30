# Setup Vulkan SDK (via system installation or FetchContent)
#
# Returns native Vulkan target and include directories via OUT parameters
# The calling project is responsible for creating ALIAS targets
#
# Parameters:
#   OUT_TARGET         - Variable name to store Vulkan target (Vulkan::Vulkan or vulkan)
#   OUT_INCLUDE_DIRS   - Variable name to store Vulkan include directories
#
# Requires:
#   - IMENGINE_VULKAN_VERSION (e.g., "1.3.296")
#   - IMENGINE_VULKAN_TAG (e.g., "v1.3.296")

include(FetchContent)

function(setup_vulkan OUT_TARGET OUT_INCLUDE_DIRS)
    
set(ML64_EXECUTABLE "")

    if(WIN32)
        file(GLOB ML64_SEARCH_PATHS
            "C:/Program Files/Microsoft Visual Studio/*/Community/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files/Microsoft Visual Studio/*/Professional/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files/Microsoft Visual Studio/*/Enterprise/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files (x86)/Microsoft Visual Studio/*/BuildTools/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
        )
        if(ML64_SEARCH_PATHS)
            list(SORT ML64_SEARCH_PATHS)
            list(GET ML64_SEARCH_PATHS -1 ML64_EXECUTABLE)
        endif()
    endif()

    if(NOT ML64_EXECUTABLE)
        find_program(ML64_EXECUTABLE ml64)
    endif()

    if(ML64_EXECUTABLE)
        message(STATUS "Found MASM assembler: ${ML64_EXECUTABLE}")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_ASM_MASM_COMPILER "${ML64_EXECUTABLE}" PARENT_SCOPE)
            set(CMAKE_ASM_MASM_COMPILER_ID "MSVC" PARENT_SCOPE)
            set(CMAKE_ASM_MASM_COMPILER_WORKS TRUE PARENT_SCOPE)
        endif()
        enable_language(ASM_MASM)
        set(USE_MASM ON CACHE BOOL "Enable MASM for Vulkan-Loader" FORCE)
    else()
        message(STATUS "MASM assembler (ml64) not found - Vulkan unknown function trampolines disabled")
        message(STATUS "  To enable: Install Visual Studio Build Tools with 'MSVC v143 x64/x86 build tools'")
        set(USE_MASM OFF CACHE BOOL "Enable MASM for Vulkan-Loader" FORCE)
    endif()

    find_package(Vulkan ${IMENGINE_VULKAN_VERSION} QUIET)

    if(TARGET Vulkan::Vulkan)
        get_target_property(is_imported Vulkan::Vulkan IMPORTED)
        if(is_imported)
            set_target_properties(Vulkan::Vulkan PROPERTIES IMPORTED_GLOBAL TRUE)
        endif()

        message(STATUS "Vulkan::Vulkan target found (SDK installed).")

        get_target_property(VULKAN_SYSTEM_INCLUDES Vulkan::Vulkan INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
        if(VULKAN_SYSTEM_INCLUDES)
            set_target_properties(Vulkan::Vulkan PROPERTIES
                INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ""
                INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_SYSTEM_INCLUDES}"
            )
        endif()

        set(${OUT_TARGET} Vulkan::Vulkan PARENT_SCOPE)
        set(${OUT_INCLUDE_DIRS} "${Vulkan_INCLUDE_DIR}" PARENT_SCOPE)
    else()
        message(STATUS "Vulkan SDK not found. Fetching Vulkan-Headers and Vulkan-Loader from GitHub...")

        FetchContent_Declare(
            Vulkan-Headers
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
            GIT_TAG ${IMENGINE_VULKAN_TAG}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(Vulkan-Headers)

        set(VULKAN_HEADERS_INCLUDE "${vulkan-headers_SOURCE_DIR}/include")
        message(STATUS "Vulkan Headers: ${VULKAN_HEADERS_INCLUDE}")

        # Generate VulkanHeadersConfig.cmake for Vulkan-Loader
        set(VULKAN_HEADERS_CONFIG_DIR "${vulkan-headers_BINARY_DIR}/VulkanHeadersConfig")
        file(MAKE_DIRECTORY "${VULKAN_HEADERS_CONFIG_DIR}")
        file(WRITE "${VULKAN_HEADERS_CONFIG_DIR}/VulkanHeadersConfig.cmake"
                "# Auto-generated VulkanHeadersConfig.cmake for FetchContent usage
                if(NOT TARGET Vulkan::Headers)
                    add_library(Vulkan::Headers ALIAS Vulkan-Headers)
                endif()
                set(VulkanHeaders_INCLUDE_DIRS \"${VULKAN_HEADERS_INCLUDE}\")
                set(VulkanHeaders_FOUND TRUE)
                ")
        set(VulkanHeaders_DIR "${VULKAN_HEADERS_CONFIG_DIR}" CACHE PATH "" FORCE)
        message(STATUS "Created VulkanHeadersConfig at: ${VULKAN_HEADERS_CONFIG_DIR}")

        set(BUILD_TESTS OFF CACHE BOOL "Build Vulkan-Loader tests" FORCE)
        set(BUILD_WSI_XCB_SUPPORT OFF CACHE BOOL "Build Vulkan-Loader XCB support" FORCE)
        set(BUILD_WSI_XLIB_SUPPORT OFF CACHE BOOL "Build Vulkan-Loader Xlib support" FORCE)
        set(BUILD_WSI_WAYLAND_SUPPORT OFF CACHE BOOL "Build Vulkan-Loader Wayland support" FORCE)
        set(ENABLE_WERROR OFF CACHE BOOL "Enable Vulkan-Loader warnings as errors" FORCE)

        FetchContent_Declare(
            Vulkan-Loader
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
            GIT_TAG ${IMENGINE_VULKAN_TAG}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(Vulkan-Loader)

        if(TARGET vulkan)
            if(MSVC AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                get_target_property(VULKAN_OPTS vulkan COMPILE_OPTIONS)
                if(VULKAN_OPTS)
                    list(REMOVE_ITEM VULKAN_OPTS "/SDL")
                    set_target_properties(vulkan PROPERTIES COMPILE_OPTIONS "${VULKAN_OPTS}")
                endif()
            endif()

            get_target_property(VULKAN_INC_DIRS vulkan INTERFACE_INCLUDE_DIRECTORIES)
            if(VULKAN_INC_DIRS)
                set(FIXED_INC_DIRS "")
                foreach(DIR ${VULKAN_INC_DIRS})
                    if(DIR MATCHES "\\$<" OR DIR MATCHES "INSTALL_INTERFACE")
                        list(APPEND FIXED_INC_DIRS "${DIR}")
                    else()
                        list(APPEND FIXED_INC_DIRS "$<BUILD_INTERFACE:${DIR}>")
                    endif()
                endforeach()
                set_target_properties(vulkan PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FIXED_INC_DIRS}")
            endif()

            # Return fetched Vulkan target and includes
            set(${OUT_TARGET} vulkan PARENT_SCOPE)
            set(${OUT_INCLUDE_DIRS} "${VULKAN_HEADERS_INCLUDE}" PARENT_SCOPE)
        else()
            message(FATAL_ERROR "Failed to build Vulkan Loader from source. Please install the Vulkan SDK.")
        endif()
    endif()
endfunction()
