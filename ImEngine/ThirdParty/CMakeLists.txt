cmake_minimum_required(VERSION 3.10)

# Définir le nom du projet
project(ImEngine-ThirdParty)

# Spécifier la version de Vulkan SDK
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")
set(Vulkan_INCLUDE_DIR "${VULKAN_SDK}/Include")
set(Vulkan_LIBRARY "${VULKAN_SDK}/Lib")

# Assurez-vous que Vulkan est trouvé
find_package(Vulkan REQUIRED)

# Inclure Vulkan
include_directories(${Vulkan_INCLUDE_DIR})
link_directories(${Vulkan_LIBRARY})

# Définir les répertoires de GLFW et ImGui
set(GLFW_DIR glfw-3.4)
set(IMGUI_DIR imgui)

# Inclure les répertoires de GLFW et ImGui
include_directories(${GLFW_DIR}/include)
include_directories(${IMGUI_DIR}/include)  # Inclure le répertoire 'include' d'ImGui
include_directories(${IMGUI_DIR}/backends)

# Désactiver la construction des exemples/tests de GLFW
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

# Ajouter GLFW en sous-répertoire
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)

# Rechercher les fichiers source
file(GLOB sources "imgui/*.cpp")

# Créer la bibliothèque ImEngine-Tools
add_library(ImEngine-ThirdParty ${sources})

# Lier les bibliothèques avec ImEngine-Tools
target_link_libraries(ImEngine-ThirdParty glfw Vulkan::Vulkan)

# Ajouter les threads si nécessaire
find_package(Threads REQUIRED)
target_link_libraries(ImEngine-ThirdParty Threads::Threads)
