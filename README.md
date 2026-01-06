# ImEngine - Modern C++23 Game Engine

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.29+-green.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> A modular game engine built with modern C++23 features, focused on performance & security.

## 🚀 Quick Start

### Prerequisites

| Tool | Version | Notes |
|------|---------|-------|
| Windows | 10/11 | Only supported platform currently |
| CMake | 3.29+ | [Download](https://cmake.org/download/) |
| Clang-CL | 19+ | [LLVM](https://releases.llvm.org/download.html) (recommended) |
| MSVC | 2022+ | [Visual Studio](https://visualstudio.microsoft.com/) (alternative) |
| Ninja | Latest | [Download](https://github.com/ninja-build/ninja/releases) |
| Git | Latest | To clone the repository |

> **Note**: ccache and Vulkan SDK are **optional** — they will be downloaded automatically if missing.

### Installation

```powershell
# 1. Clone the repository
git clone https://github.com/your-username/ImEngine.git
cd ImEngine

# 2. Configure (choose a preset)
cmake --preset editor-app

# 3. Build
cmake --build --preset editor-Debug

# 4. Run (optional)
./out/build/editor-app/bin/Debug/RUNTIME/ImEngine-Editor.exe
```

### CMake Presets

| Preset | Modules | Description |
|--------|---------|-------------|
| `engine-dll` | Core + Engine | DLLs only (no executable) |
| `sandbox-app` | Core + Engine + Sandbox | Development/testing environment |
| `editor-app` | Core + Engine + Editor | Visual editor with ImGui |

## 🎯 Architecture

### Project Structure

```
ImEngine/
├── cmake/                  # CMake modules (warnings, security, cache)
├── docs/                   # Documentation
├── ImEngine/
│   ├── Core/              # 🔧 Core library (DLL)
│   │   ├── include/       #    Memory, Concurrency, (future: Math, ECS)
│   │   ├── src/
│   │   └── misc/          #    PCH, Export macros
│   ├── Engine/            # 🎮 Main engine (DLL)
│   │   ├── include/       #    Vulkan, GLFW, game loop
│   │   ├── src/
│   │   └── misc/
│   ├── Sandbox/           # 🧪 Development and experiments
│   ├── Editor/            # 🖥️ ImGui editor
│   └── ThirdParty/        # 📦 Dependencies (GLFW, Vulkan, ImGui)
└── out/                    # Build artifacts
```

### Dependency Graph

```
ThirdParty (GLFW, Vulkan, ImGui)
         │ PUBLIC
         ▼
    ImEngine-Core ──────────────────┐
         │ PUBLIC                   │
         ▼                          │
   ImEngine-Engine                  │
         │ PRIVATE                  │
         ├──────────────┐           │
         ▼              ▼           │
      Sandbox        Editor ◄───────┘
       (EXE)         (EXE)       ImGui
```

### Modules

| Module | Type | Description |
|--------|------|-------------|
| **ImEngine::Core** | DLL | Memory allocators, Fibers, (planned: Math, ECS) |
| **ImEngine::Engine** | DLL | GLFW window, Vulkan, main loop |
| **ImEngine::Sandbox** | EXE | Development/testing environment |
| **ImEngine::Editor** | EXE | Visual editor with ImGui (docking) |

## 🔧 Features

### Modern Build System
- ✅ CMake 3.29+ with Modern CMake patterns
- ✅ Automatic ccache (downloaded if missing)
- ✅ Automatic Vulkan SDK (headers + loader)
- ✅ Precompiled headers
- ✅ Generator expressions for portability
- ✅ SDK installation and packaging

### Security
- Buffer overflow protection (`/GS`, `-fstack-protector-strong`)
- Control Flow Guard (`/guard:cf`)
- ASLR (`/DYNAMICBASE`, `-fPIE`)
- DEP (`/NXCOMPAT`)
- Strict warnings (-Wall, -Wextra, /W4)

### Performance
- Parallel compilation (Ninja Multi-Config)
- Compilation cache (ccache)
- Incremental builds < 30s

## 🛠️ Development

### Adding a New Module

1. Create folder in `ImEngine/NewModule`
2. Create `CMakeLists.txt`:
```cmake
add_library(ImEngine-NewModule SHARED
    src/main.cpp
)

set_project_warnings(ImEngine-NewModule)
enable_security_flags(ImEngine-NewModule)

target_link_libraries(ImEngine-NewModule
    PUBLIC ImEngine::Core
)

add_library(ImEngine::NewModule ALIAS ImEngine-NewModule)
```

3. Add to root `CMakeLists.txt`:
```cmake
option(IMENGINE_BUILD_NEWMODULE "Build NewModule" OFF)

if(IMENGINE_BUILD_NEWMODULE)
    add_subdirectory(ImEngine/NewModule)
endif()
```

### Build Configurations

```powershell
# Debug (no optimization, symbols)
cmake --build --preset editor-Debug

# RelWithDebInfo (optimized + symbols)
cmake --build --preset editor-RelWithDebInfo

# Release (max optimization)
cmake --build --preset editor-Release
```

## � Coding Style

ImEngine follows the **[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)** with adaptations for C++23.

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| **Types** (classes, structs, enums) | `PascalCase` | `Vector3`, `LinearAllocator` |
| **Functions** | `PascalCase` | `DotProduct()`, `InitWindow()` |
| **Variables** | `snake_case` | `extension_count`, `app_info` |
| **Class data members** | `snake_case_` (trailing underscore) | `window_`, `width_` |
| **Struct data members** | `snake_case` (no trailing underscore) | `x`, `y`, `data` |
| **Constants** | `kPascalCase` | `kRows`, `kMaxSize` |
| **Template parameters** | Single uppercase or `PascalCase` | `T`, `AngleUnit` |
| **Namespaces** | `snake_case` | `imengine::core` |
| **Macros** | `UPPER_SNAKE_CASE` | `IMENGINE_API` |
| **Accessors** | `snake_case()` | `window()`, `vulkan_instance()` |

### Formatting (.clang-format)

- **Indentation**: 2 spaces (no tabs)
- **Line length**: 80 characters
- **Braces**: Attached (`{` on same line)
- **Pointers**: `char* p` (left-aligned)

### C++23 Best Practices

```cpp
// Use [[nodiscard]] for functions returning values
[[nodiscard]] constexpr double SquaredLength() const noexcept;

// Use concepts for template constraints
template <Arithmetic T>
struct Vector3 { ... };

// Prefer constexpr for compile-time evaluation
constexpr Vector3 operator+(const Vector3& other) const noexcept;

// Use explicit for single-argument constructors
explicit constexpr Matrice(T scalar) noexcept;
```

For the complete coding guide, see [CODING_STYLE.md](docs/CODING_STYLE.md).

## 📚 Documentation

- [Coding Style Guide](docs/CODING_STYLE.md)
- [Engine Foundations Plan](docs/plan-engineFoundations.prompt.md)

## 🐛 Troubleshooting

### Error "ml64: CreateProcess failed"
✅ **Automatically resolved** - Vulkan unknown function support is disabled by default.

### ccache not found
✅ **Automatically resolved** - ccache is automatically downloaded on Windows.

### Vulkan SDK not found
✅ **Automatically resolved** - Vulkan-Headers and Vulkan-Loader are automatically downloaded and compiled.

### Slow compilation
- Check that ccache is enabled ("Using ccache" message during configure)
- Check that Ninja is being used (preferred over MSBuild)
- First compilation is always slower (download + cache warmup)

## 📝 License

MIT License - See [LICENSE](LICENSE) for more details.

---

**Built with** ❤️ **and Modern C++23**