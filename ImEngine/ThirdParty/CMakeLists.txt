cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)
include(FetchContent)

message(STATUS "[-----ImEngine-ThirdParty-----]")

if (IMENGINE_BUILD_CORE OR IMENGINE_BUILD_ENGINE OR IMENGINE_BUILD_RUNTIME OR IMENGINE_BUILD_SANDBOX)

    # =========================================================================
    # Vulkan (must be configured BEFORE GLFW so headers are available)
    # =========================================================================

    # Check if MASM assembler (ml64) is available for Vulkan-Loader assembly support
    set(ML64_EXECUTABLE "")

    # Search for ml64.exe in Visual Studio installations
    if(WIN32)
        file(GLOB ML64_SEARCH_PATHS
            "C:/Program Files/Microsoft Visual Studio/*/Community/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files/Microsoft Visual Studio/*/Professional/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files/Microsoft Visual Studio/*/Enterprise/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
            "C:/Program Files (x86)/Microsoft Visual Studio/*/BuildTools/VC/Tools/MSVC/*/bin/Hostx64/x64/ml64.exe"
        )
        if(ML64_SEARCH_PATHS)
            # Use the most recent version (last in sorted list)
            list(SORT ML64_SEARCH_PATHS)
            list(GET ML64_SEARCH_PATHS -1 ML64_EXECUTABLE)
        endif()
    endif()

    # Fallback to PATH
    if(NOT ML64_EXECUTABLE)
        find_program(ML64_EXECUTABLE ml64)
    endif()

    if(ML64_EXECUTABLE)
        message(STATUS "Found MASM assembler: ${ML64_EXECUTABLE}")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_ASM_MASM_COMPILER "${ML64_EXECUTABLE}")
            set(CMAKE_ASM_MASM_COMPILER_ID "MSVC")
            set(CMAKE_ASM_MASM_COMPILER_WORKS TRUE)
        endif()
        enable_language(ASM_MASM)
        set(USE_MASM ON CACHE BOOL "" FORCE)
    else()
        message(STATUS "MASM assembler (ml64) not found - Vulkan unknown function trampolines disabled")
        message(STATUS "  To enable: Install Visual Studio Build Tools with 'MSVC v143 x64/x86 build tools'")
        set(USE_MASM OFF CACHE BOOL "" FORCE)
    endif()

    find_package(Vulkan QUIET)
    if (TARGET Vulkan::Vulkan)
        get_target_property(is_imported Vulkan::Vulkan IMPORTED)
        if(is_imported)
            set_target_properties(Vulkan::Vulkan PROPERTIES IMPORTED_GLOBAL TRUE)
        endif()

        message(STATUS "Vulkan::Vulkan target found (SDK installed).")
        add_library(ImEngine-ThirdParty::vulkan ALIAS Vulkan::Vulkan)

        get_target_property(VULKAN_INCLUDE_DIRS Vulkan::Headers INTERFACE_INCLUDE_DIRECTORIES)
        if(NOT VULKAN_INCLUDE_DIRS)
            set(VULKAN_INCLUDE_DIRS "$ENV{VULKAN_SDK}/Include")
        endif()
    else()
        message(STATUS "Vulkan SDK not found. Fetching Vulkan-Headers and Vulkan-Loader from GitHub...")

        set(VULKAN_VERSION "v1.3.296")

        FetchContent_Declare(
            Vulkan-Headers
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
            GIT_TAG ${VULKAN_VERSION}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(Vulkan-Headers)

        # Store include path for GLFW and other consumers
        set(VULKAN_INCLUDE_DIRS "${vulkan-headers_SOURCE_DIR}/include")
        message(STATUS "Vulkan Headers: ${VULKAN_INCLUDE_DIRS}")

        set(VULKAN_HEADERS_CONFIG_DIR "${vulkan-headers_BINARY_DIR}/VulkanHeadersConfig")
        file(MAKE_DIRECTORY "${VULKAN_HEADERS_CONFIG_DIR}")
        file(WRITE "${VULKAN_HEADERS_CONFIG_DIR}/VulkanHeadersConfig.cmake"
                "# Auto-generated VulkanHeadersConfig.cmake for FetchContent usage
                if(NOT TARGET Vulkan::Headers)
                    add_library(Vulkan::Headers ALIAS Vulkan-Headers)
                endif()
                set(VulkanHeaders_INCLUDE_DIRS \"${vulkan-headers_SOURCE_DIR}/include\")
                set(VulkanHeaders_FOUND TRUE)
                ")
        set(VulkanHeaders_DIR "${VULKAN_HEADERS_CONFIG_DIR}" CACHE PATH "" FORCE)
        message(STATUS "Created VulkanHeadersConfig at: ${VULKAN_HEADERS_CONFIG_DIR}")

        # Configure Loader options
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_XCB_SUPPORT OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_XLIB_SUPPORT OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_WAYLAND_SUPPORT OFF CACHE BOOL "" FORCE)
        set(ENABLE_WERROR OFF CACHE BOOL "" FORCE)
        # Note: USE_MASM is set earlier based on ml64 availability

        FetchContent_Declare(
            Vulkan-Loader
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
            GIT_TAG ${VULKAN_VERSION}
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
                        # Already a generator expression, keep as-is
                        list(APPEND FIXED_INC_DIRS "${DIR}")
                    else()
                        list(APPEND FIXED_INC_DIRS "$<BUILD_INTERFACE:${DIR}>")
                    endif()
                endforeach()
                set_target_properties(vulkan PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FIXED_INC_DIRS}")
            endif()

            add_library(ImEngine-ThirdParty::vulkan ALIAS vulkan)
        else()
            message(FATAL_ERROR "Failed to build Vulkan Loader from source. Please install the Vulkan SDK.")
        endif()
    endif()

    # =========================================================================
    # GLFW (after Vulkan so we can provide Vulkan headers path)
    # =========================================================================
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        glfw_external
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw_external)

    # Add Vulkan headers to GLFW so it can find vulkan/vulkan.h
    if(VULKAN_INCLUDE_DIRS)
        target_include_directories(glfw PUBLIC
            $<BUILD_INTERFACE:${VULKAN_INCLUDE_DIRS}>
        )
    endif()

    add_library(ImEngine-ThirdParty::glfw ALIAS glfw)

    if (IMENGINE_BUILD_EDITOR)
        # ImGui
        FetchContent_Declare(
                imgui_external
                GIT_REPOSITORY https://github.com/ocornut/imgui.git
                GIT_TAG docking
                GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(imgui_external)

        set(IMGUI_DIR ${imgui_external_SOURCE_DIR})
        set(IMGUI_SOURCES
                ${IMGUI_DIR}/imgui.cpp
                ${IMGUI_DIR}/imgui_demo.cpp
                ${IMGUI_DIR}/imgui_draw.cpp
                ${IMGUI_DIR}/imgui_tables.cpp
                ${IMGUI_DIR}/imgui_widgets.cpp
                ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        )

        add_library(imgui STATIC ${IMGUI_SOURCES})
        target_include_directories(imgui
                PUBLIC
                $<BUILD_INTERFACE:${IMGUI_DIR}>
                $<BUILD_INTERFACE:${IMGUI_DIR}/backends>
                $<BUILD_INTERFACE:${glfw_external_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${glfw_external_SOURCE_DIR}/deps>
                $<BUILD_INTERFACE:${VULKAN_INCLUDE_DIRS}>
        )

        target_compile_definitions(imgui PUBLIC VK_PROTOTYPES)
        target_link_libraries(imgui PUBLIC ImEngine-ThirdParty::vulkan)
        add_library(ImEngine-ThirdParty::imgui ALIAS imgui)
    endif ()
endif ()