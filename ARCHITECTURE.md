# Architecture du Projet ImEngine

Ce document résume la nouvelle architecture du projet et les améliorations apportées au système de build.

## Structure du Projet

Le projet est divisé en plusieurs modules distincts pour assurer une séparation claire des responsabilités et une meilleure modularité :

*   **ImEngine-Core** (`ImEngine/Core`) :
    *   Bibliothèque partagée (DLL).
    *   Contient les systèmes de bas niveau (Gestion de la mémoire, Concurrence/Fibers, Utilitaires).
    *   Exporté via `IMENGINE_API`.

*   **ImEngine-Engine** (`ImEngine/Engine`) :
    *   Bibliothèque partagée (DLL).
    *   Contient la logique principale du moteur (Boucle de jeu, Initialisation Vulkan/GLFW, Gestion des fenêtres).
    *   Dépend de `ImEngine-Core`.
    *   Exporté via `IMENGINE_ENGINE_API`.

*   **ImEngine-Runtime** (`ImEngine/Runtime`) :
    *   Exécutable final (Application).
    *   Point d'entrée pour le jeu/application autonome.
    *   Lie `ImEngine-Engine` et `ImEngine-Core`.

*   **ImEngine-Editor** (`ImEngine/Editor`) :
    *   Exécutable (Outil).
    *   L'éditeur du moteur.
    *   Lie `ImEngine-Engine` et `ImEngine-Core`.

*   **ImEngine-Sandbox** (`ImEngine/Sandbox`) :
    *   Exécutable (Test).
    *   Environnement de test pour les développeurs.

## Système de Build (Modern CMake)

Le système de build a été entièrement refondu pour suivre les pratiques "Modern CMake" (3.29+).

### 1. Modularité et Helpers (`cmake/`)
Des scripts auxiliaires ont été créés pour centraliser la configuration :
*   **`StandardSettings.cmake`** : Définit le standard C++23, les dossiers de sortie unifiés (`bin/`), et l'export des commandes de compilation.
*   **`CompilerWarnings.cmake`** : Active des avertissements stricts (Wall, Wextra, Werror, etc.) pour MSVC, Clang et GCC afin d'assurer la qualité du code.
*   **`Security.cmake`** : Active des flags de sécurité (ASLR, DEP, Stack Protector, Control Flow Guard). Compatible avec `clang-cl` (désactive `/SDL` qui n'est pas supporté).
*   **`Cache.cmake`** : Détecte et active **ccache** s'il est disponible pour accélérer les temps de recompilation. Si ccache n'est pas installé sur Windows, il est **téléchargé automatiquement** depuis GitHub (version portable).

### 2. Gestion des Dépendances

#### Téléchargement Automatique
*   Utilisation de `FetchContent` pour gérer les dépendances tierces (GLFW, ImGui, Vulkan).
*   **ccache** : Téléchargé automatiquement si absent (Windows).
*   **Vulkan-Headers et Vulkan-Loader** : Téléchargés et compilés automatiquement si le SDK Vulkan n'est pas installé.

#### Configuration Vulkan-Loader
Pour assurer la compatibilité avec `clang-cl`, plusieurs options sont configurées :
```cmake
set(BUILD_UNKNOWN_FUNCTIONS OFF CACHE BOOL "" FORCE)  # Évite les dépendances MASM
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_WERROR OFF CACHE BOOL "" FORCE)
```

**Note importante** : Le support des "unknown functions" est désactivé pour éviter les problèmes avec l'assembleur MASM sur `clang-cl`. Cela n'affecte pas les fonctionnalités normales de Vulkan. Voir `docs/VULKAN_MASM_FIX.md` pour plus de détails.

### 3. Cibles et Alias
*   Utilisation de `target_include_directories` avec des "Generator Expressions" (`$<BUILD_INTERFACE:...>`, `$<INSTALL_INTERFACE:...>`) pour supporter l'installation et l'utilisation dans l'arbre de source.
*   Création d'alias avec namespace (ex: `ImEngine::Core`, `ImEngine::Engine`) pour une consommation uniforme par les exécutables.

### 4. Installation
*   Règles d'installation (`install(...)`) ajoutées pour les bibliothèques et les headers.
*   Permet de générer un SDK ou de déployer le moteur proprement.
*   Installation des headers d'export générés automatiquement.

### 5. Presets (`CMakePresets.json`)
*   Configuration "Ninja Multi-Config" par défaut pour des builds rapides sur Windows.
*   Presets préconfigurés :
    - `engine-only` : Core + Engine + Runtime
    - `sandbox-app` : Core + Engine + Sandbox
    - `editor-app` : Core + Engine + Editor
*   Build presets avec configurations Debug, RelWithDebInfo, Release.

## Sécurité

### Flags Activés
*   **Windows (MSVC/clang-cl)** :
    - `/GS` : Buffer Security Check
    - `/guard:cf` : Control Flow Guard
    - `/DYNAMICBASE` : ASLR (Address Space Layout Randomization)
    - `/NXCOMPAT` : DEP (Data Execution Prevention)
    - ~~`/SDL`~~ : Désactivé pour `clang-cl` (incompatibilité)

*   **Linux/GCC/Clang** :
    - `-fstack-protector-strong` : Stack smashing protection
    - `-D_FORTIFY_SOURCE=2` : Buffer overflow detection
    - `-fPIE` : Position Independent Executable
    - `-Wl,-z,relro -Wl,-z,now` : Full RELRO

## Performance

### Optimisations de Build
*   **ccache** : Cache de compilation automatique (téléchargé si nécessaire).
*   **Precompiled Headers** : Utilisés pour Core et Engine.
*   **Unity Builds** : Peut être activé via option CMake (non activé par défaut).
*   **Ninja** : Générateur par défaut pour builds parallèles rapides.

### Temps de Build Typiques (estimés)
*   **Configuration initiale** : 30-60s (téléchargement des dépendances)
*   **Build complet (Debug)** : 2-5 min (première fois)
*   **Rebuild incrémental** : 5-30s (avec ccache)

## Résumé des Changements Effectués

1.  **Refactoring Root** : Nettoyage du `CMakeLists.txt` racine, inclusion des modules helpers.
2.  **Séparation Engine/Runtime** :
    *   L'ancien code "Runtime" (librairie) a été renommé en **Engine**.
    *   Un nouvel exécutable **Runtime** a été créé qui instancie simplement l'Engine.
3.  **Sécurité & Qualité** : Application automatique des flags de sécurité et des warnings sur tous les projets.
4.  **Performance** : Intégration de `ccache` avec téléchargement automatique.
5.  **Vulkan** : Téléchargement automatique et configuration pour compatibilité `clang-cl`.
6.  **Correctifs** : Mise à jour des macros d'export et des chemins d'inclusion.

## Dépannage

### Problèmes Courants

**Erreur : `ml64: CreateProcess failed`**
→ Le support des fonctions inconnues Vulkan est désactivé par défaut. Aucune action nécessaire.

**Erreur : `ccache not found`**
→ ccache sera téléchargé automatiquement au premier build sur Windows.

**Erreur : `Vulkan SDK not found`**
→ Vulkan-Headers et Vulkan-Loader seront téléchargés et compilés automatiquement.

## Contribuer

Pour ajouter un nouveau module :
1. Créer le dossier dans `ImEngine/NouveauModule`
2. Ajouter `CMakeLists.txt` avec `add_library(ImEngine-NouveauModule ...)`
3. Appliquer les helpers : `set_project_warnings(...)` et `enable_security_flags(...)`
4. Créer l'alias : `add_library(ImEngine::NouveauModule ALIAS ImEngine-NouveauModule)`
5. Ajouter l'option dans le CMakeLists.txt racine

Cette architecture est maintenant robuste, sécurisée, rapide à compiler, et prête pour une expansion future.

