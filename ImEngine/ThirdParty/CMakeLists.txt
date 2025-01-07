cmake_minimum_required(VERSION 3.10)
project(ImEngine-ThirdParty)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# GLFW
# Assurez-vous que le répertoire de GLFW est valide
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw)

# Ajoutez GLFW comme sous-répertoire
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

# Incluez les headers GLFW
include_directories(${GLFW_DIR}/include)

# Dear ImGui
set(IMGUI_DIR imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# Libraries
find_package(Vulkan REQUIRED)
#find_library(VULKAN_LIBRARY
#NAMES vulkan vulkan-1)
#set(LIBRARIES "glfw;${VULKAN_LIBRARY}")
#set(LIBRARIES "glfw;Vulkan::Vulkan")

# Use vulkan headers from glfw:
include_directories(${GLFW_DIR}/deps)

file(GLOB sources *.cpp)

add_library(ImEngine-ThirdParty ${sources} ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
target_include_directories(ImEngine-ThirdParty
        PUBLIC
        ${GLFW_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${GLFW_DIR}/deps
)
target_link_libraries(ImEngine-ThirdParty ${GLFW_DIR}/lib-vc2010-64/glfw3.lib Vulkan::Vulkan)
