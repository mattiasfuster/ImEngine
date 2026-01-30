cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)

message(STATUS "[-----ImEngine-ThirdParty-----]")

include(ThirdParty/SetupVulkan)
include(ThirdParty/SetupGLFW)
include(ThirdParty/SetupImGui)

if(IMENGINE_BUILD_CORE OR IMENGINE_BUILD_ENGINE OR IMENGINE_BUILD_SANDBOX)

    setup_vulkan(VULKAN_TARGET VULKAN_INCLUDE_DIRS)
    add_library(ImEngine-ThirdParty::vulkan ALIAS ${VULKAN_TARGET})

    setup_glfw(GLFW_TARGET "${VULKAN_INCLUDE_DIRS}")
    add_library(ImEngine-ThirdParty::glfw ALIAS ${GLFW_TARGET})


    if(IMENGINE_BUILD_EDITOR)
        setup_imgui(
            IMGUI_TARGET
            "${VULKAN_TARGET}"
            "${GLFW_TARGET}"
            "${VULKAN_INCLUDE_DIRS}"
            "${glfw_external_SOURCE_DIR}"
        )

        add_library(ImEngine-ThirdParty::imgui ALIAS ${IMGUI_TARGET})
    endif()
endif()
