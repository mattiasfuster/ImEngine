cmake_minimum_required(VERSION 3.29)
project(ImEngine-ThirdParty)

# Force Debug build type par défaut (utile pour développement uniquement)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# C++20 requis
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Répertoire GLFW
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4)

# Options GLFW désactivées
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

# Ajouter GLFW
add_subdirectory(${GLFW_DIR})

# Répertoire ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# Sources ImGui + backends Vulkan & GLFW
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

# Recherche Vulkan SDK
find_package(Vulkan REQUIRED)

# Inclure les éventuels .cpp supplémentaires
file(GLOB THIRD_PARTY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Créer une bibliothèque statique (plus simple pour ImGui)
add_library(ImEngine-ThirdParty STATIC
        ${THIRD_PARTY_SOURCES}
        ${IMGUI_SOURCES}
)

# Include dirs
target_include_directories(ImEngine-ThirdParty
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${GLFW_DIR}/include
        PUBLIC ${GLFW_DIR}/deps
)

# Lien avec GLFW et Vulkan
target_link_libraries(ImEngine-ThirdParty
        glfw
        Vulkan::Vulkan
)

# Définir VK_PROTOTYPES pour les headers Vulkan (optionnel)
target_compile_definitions(ImEngine-ThirdParty PRIVATE VK_PROTOTYPES)
