# ImEngine Copilot Instructions

## Architecture Overview

ImEngine is a modular C++23 game engine built as DLLs (Windows only). The dependency hierarchy is:

```
ThirdParty (GLFW, Vulkan) → Core (DLL) → Engine (DLL) → Sandbox/Editor (EXE)
```

- **Core** (`ImEngine/Core/`): Memory allocators, Math (Vector, Matrix, Quaternion), Concurrency (Fibers planned)
- **Engine** (`ImEngine/Engine/`): GLFW window management, Vulkan initialization, main game loop
- **Sandbox/Editor**: Executable entry points linking to Engine

Each module uses CMake alias targets (e.g., `ImEngine::Core`, `ImEngine::Engine`) for clean dependency declaration.

## Build System

Configure and build using CMake presets:
```powershell
cmake --preset sandbox-app          # Configure (choose: engine-dll, sandbox-app, editor-app)
cmake --build --preset sandbox-Debug  # Build (Debug/RelWithDebInfo/Release)
```

Key build features:
- Clang-CL 19+ compiler (LLVM) with Ninja generator
- Vulkan SDK and ccache are auto-downloaded if missing
- Precompiled headers in `misc/ImEngine-*.pch.h` per module
- Export macros generated in `misc/*Export.h` (e.g., `IMENGINE_API`, `IMENGINE_ENGINE_API`)

## Code Conventions (Google Style + C++23)

### Naming
| Element | Convention | Example |
|---------|------------|---------|
| Types | `PascalCase` | `LinearAllocator`, `Vector3` |
| Functions | `PascalCase` | `DotProduct()`, `InitWindow()` |
| Variables | `snake_case` | `extension_count` |
| Class members | `snake_case_` (trailing `_`) | `window_`, `width_` |
| Struct members | `snake_case` (no trailing) | `x`, `y` |
| Constants | `kPascalCase` | `kMaxSize` |
| Namespaces | `snake_case` | `ImEngine::Core::Maths` |

### C++23 Patterns
```cpp
// Use concepts for constraints (see Maths/Concepts.h)
template <Arithmetic T>
struct Vector3 { ... };

// Always use [[nodiscard]] for value-returning functions
[[nodiscard]] constexpr double SquaredLength() const noexcept;

// Prefer constexpr for compile-time evaluation
constexpr Vector3 operator+(const Vector3& other) const noexcept;

// Accessors: snake_case(), mutators: set_snake_case()
[[nodiscard]] GLFWwindow* window() const { return window_; }
```

### Include Order
1. Associated header
2. C system headers
3. C++ standard library
4. Third-party libraries
5. Project headers

### Include Guards
Format: `PROJECT_PATH_FILE_H_` (e.g., `IMENGINE_MATHS_OBJECTS_VECTOR_H_`)

## Adding New Code

### New Module
1. Create `ImEngine/NewModule/` with `CMakeLists.txt`, `include/`, `src/`, `misc/`
2. Use `set_project_warnings()` and `enable_security_flags()` from [cmake/](../cmake/)
3. Add option `IMENGINE_BUILD_NEWMODULE` in root CMakeLists.txt
4. Link with `PUBLIC ImEngine::Core` (or appropriate dependency)

### New Class in Core
- Header-only math types in `Core/include/Maths/Objects/`
- Memory allocators inherit from `MemoryAllocatorBase<Derived>` (CRTP pattern)
- Concepts go in `Maths/Concepts.h`

## Key Files
- [CMakePresets.json](../CMakePresets.json) - Build configurations
- [cmake/CompilerWarnings.cmake](../cmake/CompilerWarnings.cmake) - Warning flags
- [cmake/Security.cmake](../cmake/Security.cmake) - Security hardening flags
- [docs/CODING_STYLE.md](../docs/CODING_STYLE.md) - Full style guide (English)
- [ImEngine/Core/include/Maths/Concepts.h](../ImEngine/Core/include/Maths/Concepts.h) - Custom concepts
