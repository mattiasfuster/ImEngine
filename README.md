# ImEngine - Modern C++23 Game Engine

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.29+-green.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> Un moteur de jeu modulaire construit avec les fonctionnalités modernes de C++23, axé sur la sécurité, la performance et l'apprentissage.

## 🚀 Démarrage Rapide

### Prérequis

| Outil | Version | Notes |
|-------|---------|-------|
| Windows | 10/11 | Seule plateforme supportée actuellement |
| CMake | 3.29+ | [Télécharger](https://cmake.org/download/) |
| Clang-CL | 19+ | [LLVM](https://releases.llvm.org/download.html) (recommandé) |
| MSVC | 2022+ | [Visual Studio](https://visualstudio.microsoft.com/fr/) (alternatif) |
| Ninja | Latest | [Télécharger](https://github.com/ninja-build/ninja/releases) |
| Git | Latest | Pour cloner le repository |

> **Note** : ccache et Vulkan SDK sont **optionnels** — ils seront téléchargés automatiquement si absents.

### Installation

```powershell
# 1. Cloner le repository
git clone https://github.com/votre-username/ImEngine.git
cd ImEngine

# 2. Configurer (choisir un preset)
cmake --preset editor-app

# 3. Compiler
cmake --build --preset editor-Debug

# 4. Exécuter (optionnel)
./out/build/editor-app/bin/Debug/RUNTIME/ImEngine-Editor.exe
```

### Presets CMake

| Preset | Modules | Description |
|--------|---------|-------------|
| `engine-only` | Core + Engine + Runtime | Application standalone minimale |
| `editor-app` | Core + Engine + Editor | Éditeur visuel avec ImGui |
| `sandbox-app` | Core + Engine + Sandbox | Environnement de test/expérimentation |

## 🎯 Architecture

### Structure du Projet

```
ImEngine/
├── cmake/                  # Modules CMake (warnings, sécurité, cache)
├── docs/                   # Documentation
├── ImEngine/
│   ├── Core/              # 🔧 Bibliothèque fondamentale (DLL)
│   │   ├── include/       #    Memory, Concurrency, (futur: Math, ECS)
│   │   ├── src/
│   │   └── misc/          #    PCH, Export macros
│   ├── Engine/            # 🎮 Moteur principal (DLL)
│   │   ├── include/       #    Vulkan, GLFW, boucle de jeu
│   │   ├── src/
│   │   └── misc/
│   ├── Runtime/           # ▶️ Exécutable standalone
│   ├── Editor/            # 🖥️ Éditeur ImGui
│   ├── Sandbox/           # 🧪 Tests et expérimentations
│   └── ThirdParty/        # 📦 Dépendances (GLFW, Vulkan, ImGui)
└── out/                    # Build artifacts
```

### Graphe des Dépendances

```
ThirdParty (GLFW, Vulkan, ImGui)
         │ PUBLIC
         ▼
    ImEngine-Core ──────────────────┐
         │ PUBLIC                   │
         ▼                          │
   ImEngine-Engine                  │
         │ PRIVATE                  │
    ┌────┴────┬──────────┐          │
    ▼         ▼          ▼          │
 Runtime   Sandbox    Editor ◄──────┘
  (EXE)     (EXE)     (EXE)      ImGui
```

### Modules

| Module | Type | Description |
|--------|------|-------------|
| **ImEngine::Core** | DLL | Allocateurs mémoire, Fibers, (prévu: Math, ECS) |
| **ImEngine::Engine** | DLL | Fenêtre GLFW, Vulkan, boucle principale |
| **ImEngine::Runtime** | EXE | Point d'entrée application standalone |
| **ImEngine::Sandbox** | EXE | Environnement de test/expérimentation |
| **ImEngine::Editor** | EXE | Éditeur visuel avec ImGui (docking) |

## 🔧 Fonctionnalités

### Build System Moderne
- ✅ CMake 3.29+ avec Modern CMake patterns
- ✅ ccache automatique (téléchargement si absent)
- ✅ Vulkan SDK automatique (headers + loader)
- ✅ Precompiled headers
- ✅ Generator expressions pour portabilité
- ✅ Installation et packaging SDK

### Sécurité
- Buffer overflow protection (`/GS`, `-fstack-protector-strong`)
- Control Flow Guard (`/guard:cf`)
- ASLR (`/DYNAMICBASE`, `-fPIE`)
- DEP (`/NXCOMPAT`)
- Warnings stricts (-Wall, -Wextra, /W4)

### Performance
- Compilation parallèle (Ninja Multi-Config)
- Cache de compilation (ccache)
- Builds incrémentaux < 30s

## 🛠️ Développement

### Ajouter un Nouveau Module

1. Créer le dossier dans `ImEngine/NouveauModule`
2. Créer `CMakeLists.txt` :
```cmake
add_library(ImEngine-NouveauModule SHARED
    src/main.cpp
)

set_project_warnings(ImEngine-NouveauModule)
enable_security_flags(ImEngine-NouveauModule)

target_link_libraries(ImEngine-NouveauModule
    PUBLIC ImEngine::Core
)

add_library(ImEngine::NouveauModule ALIAS ImEngine-NouveauModule)
```

3. Ajouter au `CMakeLists.txt` racine :
```cmake
option(IMENGINE_BUILD_NOUVEAUMODULE "Build NouveauModule" OFF)

if(IMENGINE_BUILD_NOUVEAUMODULE)
    add_subdirectory(ImEngine/NouveauModule)
endif()
```

### Configurations de Build

```powershell
# Debug (optimisation off, symboles)
cmake --build --preset editor-Debug

# RelWithDebInfo (optimisé + symboles)
cmake --build --preset editor-RelWithDebInfo

# Release (optimisation max)
cmake --build --preset editor-Release
```

## 📚 Documentation

- [Architecture Détaillée](docs/ARCHITECTURE.md)
- [Plan Engine Foundations](docs/plan-engineFoundations.prompt.md)

## 🐛 Dépannage

### Erreur "ml64: CreateProcess failed"
✅ **Résolu automatiquement** - Le support des fonctions inconnues Vulkan est désactivé par défaut.

### ccache not found
✅ **Résolu automatiquement** - ccache est téléchargé automatiquement sur Windows.

### Vulkan SDK not found
✅ **Résolu automatiquement** - Vulkan-Headers et Vulkan-Loader sont téléchargés et compilés automatiquement.

### Compilation lente
- Vérifiez que ccache est activé (message "Using ccache" au configure)
- Vérifiez que Ninja est utilisé (préféré à MSBuild)
- Première compilation toujours plus longue (téléchargement + cache)

## 📝 Licence

MIT License - Voir [LICENSE](LICENSE) pour plus de détails.

## 🤝 Contribution

Les contributions sont les bienvenues ! Consultez [ARCHITECTURE.md](docs/ARCHITECTURE.md) pour comprendre la structure du projet.

---

**Développé avec** ❤️ **et Modern C++23**

