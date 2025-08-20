cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# =============================
# GLFW
# =============================
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS    "Build the GLFW test programs"    OFF)
option(GLFW_BUILD_DOCS     "Build the GLFW documentation"    OFF)
option(GLFW_INSTALL        "Generate installation target"    OFF)

FetchContent_Declare(
        glfw_external
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw_external)

# =============================
# ImGui
# =============================
FetchContent_Declare(
        imgui_external
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui_external)

# On redéfinit IMGUI_DIR vers le dossier fetché
set(IMGUI_DIR ${imgui_external_SOURCE_DIR})

# Et on ajoute les sources manuellement (comme tu fais déjà)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

# =============================
# Vulkan
# =============================
find_package(Vulkan REQUIRED)

# =============================
# Build third-party lib
# =============================
file(GLOB THIRD_PARTY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(ImEngine-ThirdParty STATIC
        ${THIRD_PARTY_SOURCES}
        ${IMGUI_SOURCES}
)

target_include_directories(ImEngine-ThirdParty
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${glfw_external_SOURCE_DIR}/include
        PUBLIC ${glfw_external_SOURCE_DIR}/deps
)

target_link_libraries(ImEngine-ThirdParty
        glfw
        Vulkan::Vulkan
)

target_compile_definitions(ImEngine-ThirdParty PRIVATE VK_PROTOTYPES)
