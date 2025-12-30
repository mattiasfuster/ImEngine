# ImEngine - Architecture

## 📁 Structure du Projet

```
ImEngine/
├── cmake/                      # Modules CMake réutilisables
│   ├── StandardSettings.cmake  # C++23, output directories
│   ├── CompilerWarnings.cmake  # Warnings stricts
│   ├── Security.cmake          # Flags de sécurité (ASLR, DEP, CFG)
│   ├── Cache.cmake             # Support ccache auto-download
│   └── PatchVulkanLoader.cmake # Corrections Vulkan-Loader
│
├── ImEngine/
│   ├── ThirdParty/            # Dépendances externes (FetchContent)
│   │   └── CMakeLists.txt     # GLFW, Vulkan, ImGui
│   │
│   ├── Core/                  # 🔧 Bibliothèque fondamentale (DLL)
│   │   ├── include/           # Headers publics
│   │   │   ├── Concurrency/   # Fiber system
│   │   │   └── Memory/        # Allocateurs mémoire
│   │   ├── src/               # Implémentation
│   │   └── misc/              # PCH, Export macros
│   │
│   ├── Engine/                # 🎮 Moteur principal (DLL)
│   │   ├── include/           # API du moteur
│   │   ├── src/               # Implémentation
│   │   └── misc/              # PCH, Export macros
│   │
│   ├── Runtime/               # ▶️ Exécutable standalone
│   │   └── src/main.cpp
│   │
│   ├── Sandbox/               # 🧪 Application de test
│   │   └── src/
│   │
│   └── Editor/                # 🖥️ Éditeur ImGui
│       ├── include/
│       ├── src/
│       └── misc/
│
├── docs/                      # Documentation
│   ├── ARCHITECTURE.md        # Ce fichier
│   └── plan-*.prompt.md       # Plans de développement
│
└── out/                       # Build artifacts (généré)
    ├── build/                 # Fichiers de build CMake
    └── install/               # Fichiers installés
```

## 🔗 Graphe des Dépendances

```
┌─────────────────┐
│   ThirdParty    │
│  (glfw, vulkan) │
└────────┬────────┘
         │ PUBLIC
         ▼
┌─────────────────┐
│  ImEngine-Core  │  ← Bibliothèque fondamentale
│     (DLL)       │    Memory, Concurrency, Utils
└────────┬────────┘
         │ PUBLIC
         ▼
┌─────────────────┐
│ ImEngine-Engine │  ← Moteur de jeu
│     (DLL)       │    Rendering, ECS, Physics
└────────┬────────┘
         │ PRIVATE
    ┌────┴────┬──────────┐
    ▼         ▼          ▼
┌───────┐ ┌───────┐ ┌────────┐
│Runtime│ │Sandbox│ │ Editor │
│ (EXE) │ │ (EXE) │ │ (EXE)  │
└───────┘ └───────┘ └────────┘
                         │
                    ┌────┴────┐
                    │  ImGui  │
                    └─────────┘
```

## 📋 Conventions

### Nommage des fichiers
- **PCH**: `ImEngine-<Module>.pch.h`
- **Export**: `<Module>Export.h`
- **Alias CMake**: `ImEngine::<Module>`

### Structure d'un module
```
<Module>/
├── CMakeLists.txt
├── include/           # Headers publics (API)
│   └── <Feature>/
│       └── <Class>.h
├── src/               # Implémentation privée
│   └── <Feature>/
│       └── <Class>.cpp
└── misc/              # Fichiers auxiliaires
    ├── ImEngine-<Module>.pch.h
    └── <Module>Export.h
```

### Dépendances transitives
- Les dépendances `PUBLIC` sont propagées automatiquement
- **Ne pas dupliquer** les dépendances déjà présentes via une bibliothèque parente
- Exemple: Runtime link Engine → Core, glfw, vulkan sont automatiquement disponibles

## 🛠️ Presets CMake

| Preset | Description |
|--------|-------------|
| `engine-only` | Core + Engine + Runtime |
| `sandbox-app` | Core + Engine + Sandbox |
| `editor-app` | Core + Engine + Editor + ImGui |

## 🔒 Sécurité

Tous les modules incluent automatiquement:
- **Windows**: `/GS`, `/GUARD:CF`, `/DYNAMICBASE`, `/NXCOMPAT`
- **Linux/macOS**: `-fstack-protector-strong`, `-fPIE`, `FORTIFY_SOURCE=2`

