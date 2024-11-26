cmake_minimum_required(VERSION 3.29)
project(ImEngine-Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration de la bibliothèque PCH
add_library(ImEngine-Engine-PCH INTERFACE)
target_precompile_headers(ImEngine-Engine-PCH INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)

# Ajouter les fichiers source et header pour la bibliothèque
set(HEADER_FILES
        include/Task.h
)

set(SOURCE_FILES
        src/Task.cpp
)

# Créez la bibliothèque partagée ImEngine-Engine
add_library(ImEngine-Engine SHARED ${HEADER_FILES} ${SOURCE_FILES})

# Lier le PCH à la bibliothèque ImEngine-Engine
target_link_libraries(ImEngine-Engine PRIVATE ImEngine-Engine-PCH)

# Définir les symboles d'exportation pour la DLL
target_compile_definitions(ImEngine-Engine PRIVATE IMENGINE_DLL_EXPORTS)

# Ajouter le chemin d'inclusion pour les fichiers d'en-tête
target_include_directories(ImEngine-Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/pch
)

# Installation de la bibliothèque
install(TARGETS ImEngine-Engine
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib/static
)
