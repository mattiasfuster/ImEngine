# Setup ImGui (immediate mode GUI library with GLFW and Vulkan backends)
#
# Returns native ImGui target via OUT parameter
# The calling project is responsible for creating ALIAS targets
#
# Parameters:
#   OUT_TARGET          - Variable name to store ImGui target (imgui)
#   VULKAN_TARGET       - Vulkan target to link against (from setup_vulkan)
#   GLFW_TARGET         - GLFW target to link against (from setup_glfw)
#   VULKAN_INCLUDE_DIRS - Vulkan include directories (from setup_vulkan)
#   GLFW_SOURCE_DIR     - GLFW source directory (from setup_glfw via glfw_external_SOURCE_DIR)

include(FetchContent)

function(setup_imgui OUT_TARGET VULKAN_TARGET GLFW_TARGET VULKAN_INCLUDE_DIRS GLFW_SOURCE_DIR)
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
            $<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/deps>
            $<BUILD_INTERFACE:${VULKAN_INCLUDE_DIRS}>
    )

    target_compile_definitions(imgui PUBLIC VK_PROTOTYPES)

    target_link_libraries(imgui
        PUBLIC ${VULKAN_TARGET}
        PUBLIC ${GLFW_TARGET}
    )

    set(${OUT_TARGET} imgui PARENT_SCOPE)
endfunction()
