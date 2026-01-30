# Setup GLFW (windowing and input library)
#
# Returns native GLFW target via OUT parameter
# The calling project is responsible for creating ALIAS targets
#
# Parameters:
#   OUT_TARGET          - Variable name to store GLFW target (glfw)
#   VULKAN_INCLUDE_DIRS - Vulkan include directories (from setup_vulkan)

include(FetchContent)

function(setup_glfw OUT_TARGET VULKAN_INCLUDE_DIRS)
    # Configure GLFW build options (disable examples, tests, docs)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "Install GLFW" FORCE)

    # Fetch GLFW from GitHub
    FetchContent_Declare(
        glfw_external
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw_external)

    # Add Vulkan headers to GLFW so it can find vulkan/vulkan.h
    # GLFW needs Vulkan headers for its Vulkan surface creation functions
    if(VULKAN_INCLUDE_DIRS)
        target_include_directories(glfw PUBLIC
            $<BUILD_INTERFACE:${VULKAN_INCLUDE_DIRS}>
        )
    endif()

    # Return native GLFW target
    set(${OUT_TARGET} glfw PARENT_SCOPE)
endfunction()
