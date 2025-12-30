# ImEngine - Modern C++ Game Engine

Un moteur de jeu moderne en C++23 avec architecture modulaire, build system optimisé et support Vulkan.

## 🚀 Démarrage Rapide

### Prérequis

**Windows :**
- Windows 10/11
- CMake 3.29+ ([Télécharger](https://cmake.org/download/))
- Clang-CL 19+ ou MSVC 2022+ ([LLVM](https://releases.llvm.org/download.html) ou [Visual Studio](https://visualstudio.microsoft.com/fr/))
- Ninja ([Télécharger](https://github.com/ninja-build/ninja/releases))
- Git

**Note** : ccache et Vulkan SDK sont optionnels - ils seront téléchargés automatiquement si absents.

### Compilation

```powershell
# Cloner le repository
git clone https://github.com/votre-username/ImEngine.git
cd ImEngine

# Option 1 : Utiliser le script de test
.\test-build.ps1 editor-app

# Option 2 : Commandes manuelles
cmake --preset editor-app
cmake --build --preset editor-Debug
```

### Presets Disponibles

- `engine-only` : Core + Engine + Runtime (application de base)
- `editor-app` : Core + Engine + Editor (éditeur complet avec ImGui)
- `sandbox-app` : Core + Engine + Sandbox (environnement de test)

## 📁 Structure du Projet

```
ImEngine/
├── cmake/                      # Scripts CMake auxiliaires
│   ├── Cache.cmake            # Configuration ccache
│   ├── CompilerWarnings.cmake # Warnings stricts
│   ├── Security.cmake         # Flags de sécurité
│   └── StandardSettings.cmake # Standards C++
├── ImEngine/
│   ├── Core/                  # Bibliothèque de base (DLL)
│   ├── Engine/                # Logique moteur (DLL)
│   ├── Runtime/               # Application de jeu (EXE)
│   ├── Editor/                # Éditeur ImGui (EXE)
│   ├── Sandbox/               # Tests (EXE)
│   └── ThirdParty/            # Dépendances externes
├── docs/                      # Documentation
├── out/                       # Dossier de build (généré)
├── CMakeLists.txt             # Configuration racine
├── CMakePresets.json          # Presets de configuration
└── ARCHITECTURE.md            # Documentation d'architecture
```

## 🎯 Caractéristiques

### Architecture Modulaire
- **ImEngine::Core** : Systèmes de base (mémoire, concurrence)
- **ImEngine::Engine** : Logique moteur (Vulkan, GLFW, boucle de jeu)
- **ImEngine::Runtime** : Point d'entrée application
- **ImEngine::Editor** : Éditeur visuel avec ImGui

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

- [Architecture Détaillée](ARCHITECTURE.md)
- [Fix Vulkan MASM](docs/VULKAN_MASM_FIX.md)

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

Les contributions sont les bienvenues ! Consultez [ARCHITECTURE.md](ARCHITECTURE.md) pour comprendre la structure du projet.

---

**Développé avec** ❤️ **et Modern C++23**

