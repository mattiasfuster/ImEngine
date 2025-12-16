cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)
include(FetchContent)

message(STATUS "[-----ImEngine-ThirdParty-----]")

if (IMENGINE_BUILD_ENGINE OR IMENGINE_BUILD_SANDBOX)
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
    find_package(Vulkan REQUIRED GLOBAL)
    if (TARGET Vulkan::Vulkan)
        message(STATUS "Vulkan::Vulkan target found.")
        add_library(ImEngine-ThirdParty::vulkan ALIAS Vulkan::Vulkan)
    else()
        message(FATAL_ERROR "Vulkan::Vulkan target not found.")
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