cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)
include(FetchContent)

message(STATUS "[-----ImEngine-ThirdParty-----]")

if (IMENGINE_BUILD_CORE OR IMENGINE_BUILD_ENGINE OR IMENGINE_BUILD_RUNTIME OR IMENGINE_BUILD_SANDBOX)
    # GLFW
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_INSTALL OFF)
    FetchContent_Declare(
            glfw_external
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw_external)
    add_library(ImEngine-ThirdParty::glfw ALIAS glfw)

    # Vulkan
    enable_language(ASM_MASM)
    find_package(Vulkan QUIET)
    if (TARGET Vulkan::Vulkan)
        message(STATUS "Vulkan::Vulkan target found.")
        add_library(ImEngine-ThirdParty::vulkan ALIAS Vulkan::Vulkan)
    else()
        message(STATUS "Vulkan SDK not found. Fetching Vulkan-Headers and Vulkan-Loader from GitHub...")

        set(VULKAN_VERSION "v1.3.296")

        # 1. Headers
        FetchContent_Declare(
            Vulkan-Headers
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
            GIT_TAG ${VULKAN_VERSION}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(Vulkan-Headers)

        # 2. Loader
        # Help Vulkan-Loader find the headers
        set(VULKAN_HEADERS_INSTALL_DIR "${Vulkan-Headers_SOURCE_DIR}" CACHE PATH "" FORCE)

        # Configure Loader options
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_XCB_SUPPORT OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_XLIB_SUPPORT OFF CACHE BOOL "" FORCE)
        set(BUILD_WSI_WAYLAND_SUPPORT OFF CACHE BOOL "" FORCE)
        set(ENABLE_WERROR OFF CACHE BOOL "" FORCE)
        # Disable unknown functions support to avoid MASM dependency issues with clang-cl
        set(BUILD_UNKNOWN_FUNCTIONS OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            Vulkan-Loader
            GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
            GIT_TAG ${VULKAN_VERSION}
            GIT_SHALLOW TRUE
        )

        FetchContent_MakeAvailable(Vulkan-Loader)

        if(TARGET vulkan)
            # Fix clang-cl /SDL issue in Vulkan-Loader
            if(MSVC AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                get_target_property(VULKAN_OPTS vulkan COMPILE_OPTIONS)
                if(VULKAN_OPTS)
                    list(REMOVE_ITEM VULKAN_OPTS "/SDL")
                    set_target_properties(vulkan PROPERTIES COMPILE_OPTIONS "${VULKAN_OPTS}")
                endif()
            endif()

            add_library(ImEngine-ThirdParty::vulkan ALIAS vulkan)
            # Ensure headers are propagated
            target_include_directories(vulkan PUBLIC "${Vulkan-Headers_SOURCE_DIR}/include")
        else()
             message(FATAL_ERROR "Failed to build Vulkan Loader from source. Please install the Vulkan SDK.")
        endif()
    endif()

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
                ${IMGUI_DIR}
                ${IMGUI_DIR}/backends
                ${glfw_external_SOURCE_DIR}/include
                ${glfw_external_SOURCE_DIR}/deps
        )

        target_compile_definitions(imgui PUBLIC VK_PROTOTYPES)
        add_library(ImEngine-ThirdParty::imgui ALIAS imgui)
    endif ()
endif ()