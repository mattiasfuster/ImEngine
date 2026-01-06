# ImEngine Coding Style Guide

This document describes ImEngine's coding conventions, based on the **[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)** adapted for C++23.

## Table of Contents

1. [Naming Conventions](#naming-conventions)
2. [Formatting](#formatting)
3. [Headers and Includes](#headers-and-includes)
4. [Classes and Structs](#classes-and-structs)
5. [Functions](#functions)
6. [Constants and constexpr](#constants-and-constexpr)
7. [Templates and Concepts](#templates-and-concepts)
8. [Comments](#comments)
9. [C++23 Best Practices](#c23-best-practices)

---

## Naming Conventions

### Summary Table

| Element | Convention | Example |
|---------|------------|---------|
| **Types** (classes, structs, enums) | `PascalCase` | `Vector3`, `LinearAllocator` |
| **Functions** | `PascalCase` | `DotProduct()`, `InitWindow()` |
| **Local variables** | `snake_case` | `extension_count`, `app_info` |
| **Class members** | `snake_case_` (trailing underscore) | `window_`, `width_` |
| **Struct members** | `snake_case` (no underscore) | `x`, `y`, `data` |
| **Constants** | `kPascalCase` | `kRows`, `kMaxSize` |
| **Template parameters** | Uppercase or `PascalCase` | `T`, `AngleUnit` |
| **Namespaces** | `snake_case` | `imengine::core` |
| **Macros** | `UPPER_SNAKE_CASE` | `IMENGINE_API`, `IM_INFO` |
| **Accessors** | `snake_case()` | `window()`, `vulkan_instance()` |

### Types (Classes, Structs, Enums)

```cpp
// ✅ Correct
class Engine { };
struct Vector3 { };
enum class UrlTableError { kOk, kNotFound };

// ❌ Incorrect
class engine { };
struct vector_3 { };
enum class URLTableError { };  // No uppercase acronyms
```

### Variables

```cpp
// Local variables: snake_case
int extension_count = 0;
std::string table_name;

// Class members: snake_case with trailing underscore
class MyClass {
 private:
  int count_;
  std::string name_;
};

// Struct members: snake_case without underscore
struct Point {
  float x;
  float y;
};
```

### Constants

```cpp
// Constants with 'k' prefix
constexpr int kDaysInAWeek = 7;
constexpr size_t kMaxBufferSize = 1024;

// Static class constants
class Matrice {
  static constexpr size_t kRows = 4;
  static constexpr size_t kCols = 4;
};
```

### Functions and Methods

```cpp
// Functions: PascalCase
void InitWindow();
double DotProduct(const Vector3& a, const Vector3& b);

// Accessors: snake_case (modern Google style)
class Engine {
 public:
  [[nodiscard]] GLFWwindow* window() const { return window_; }
  void set_window(GLFWwindow* window) { window_ = window; }
};
```

---

## Formatting

### .clang-format Configuration

```yaml
BasedOnStyle: Google
Standard: c++23
ColumnLimit: 80
IndentWidth: 2
UseTab: Never
BreakBeforeBraces: Attach
PointerAlignment: Left
```

### Indentation

- **2 spaces** for indentation
- **No tabs**
- Access modifiers indented by **1 space** relative to the class

```cpp
class MyClass {
 public:  // 1 space
  void Foo();  // 2 spaces
  
 private:
  int bar_;
};
```

### Braces

**Attached** style (K&R): the opening brace is on the same line.

```cpp
// ✅ Correct
if (condition) {
  DoSomething();
}

void Function() {
  // code
}

class MyClass {
 public:
  void Method();
};

// ❌ Incorrect (Allman style)
if (condition)
{
  DoSomething();
}
```

### Line Length

- Maximum **80 characters** per line
- Exceptions: URLs, file paths, non-splittable strings

### Pointers and References

```cpp
// ✅ Correct: left-aligned
char* name;
const std::string& ref;
int* GetPointer();

// ❌ Incorrect
char *name;
char * name;
```

---

## Headers and Includes

### Include Order

1. Associated header (foo.h for foo.cc)
2. Blank line
3. C system headers (with `.h` extension)
4. Blank line
5. C++ standard library headers
6. Blank line
7. Third-party library headers
8. Blank line
9. Project headers

```cpp
#include "Engine.h"

#include <stdlib.h>

#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#include "Debug/Logger.h"
```

### Include Guards

Format: `PROJECT_PATH_FILE_H_`

```cpp
#ifndef IMENGINE_MATHS_OBJECTS_VECTOR_H_
#define IMENGINE_MATHS_OBJECTS_VECTOR_H_

// content

#endif  // IMENGINE_MATHS_OBJECTS_VECTOR_H_
```

---

## Classes and Structs

### When to Use struct vs class

- **struct**: passive data without invariants (POD-like)
- **class**: with invariants, encapsulation, non-trivial methods

```cpp
// struct: simple data
struct Vector3 {
  float x;
  float y;
  float z;
};

// class: with invariants and encapsulation
class Engine {
 public:
  void Run();
  
 private:
  GLFWwindow* window_ = nullptr;
};
```

### Declaration Order

```cpp
class MyClass {
 public:
  // 1. Types and aliases
  using ValueType = int;
  
  // 2. Static constants
  static constexpr int kMaxSize = 100;
  
  // 3. Constructors and assignment operators
  MyClass();
  MyClass(const MyClass&) = default;
  MyClass& operator=(const MyClass&) = default;
  
  // 4. Destructor
  ~MyClass();
  
  // 5. Methods
  void DoSomething();
  [[nodiscard]] int value() const { return value_; }
  
 protected:
  // Protected members
  
 private:
  // 6. Data members
  int value_ = 0;
};
```

### Copy and Move

Explicitly declare copy/move operations:

```cpp
class Copyable {
 public:
  Copyable(const Copyable&) = default;
  Copyable& operator=(const Copyable&) = default;
};

class MoveOnly {
 public:
  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;
};
```

---

## Functions

### Parameters

```cpp
// Inputs: value or const reference
void Process(int value);
void Process(const std::string& name);

// Outputs: prefer return by value
[[nodiscard]] std::string GetName() const;

// Input/Output: non-const reference or pointer
void Modify(std::string& name);
void Modify(std::string* name);  // if can be null
```

### Short Functions

- Keep functions short and focused
- Approximately **40 lines** maximum
- Extract subtasks into separate functions

### [[nodiscard]]

Use `[[nodiscard]]` for functions whose return value should not be ignored:

```cpp
[[nodiscard]] constexpr double SquaredLength() const noexcept;
[[nodiscard]] bool IsValid() const;
[[nodiscard]] std::unique_ptr<Widget> CreateWidget();
```

---

## Constants and constexpr

### Prefer constexpr over const

```cpp
// ✅ Preferred for compile-time known values
constexpr int kMaxSize = 1024;
constexpr double kPi = 3.14159265358979323846;

// const for runtime values
const int size = ComputeSize();
```

### constexpr Functions

```cpp
template <Arithmetic T>
struct Vector3 {
  T x, y, z;
  
  [[nodiscard]] constexpr double SquaredLength() const noexcept {
    return static_cast<double>(x * x + y * y + z * z);
  }
  
  [[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const
      noexcept {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }
};
```

---

## Templates and Concepts

### Concepts (C++20/23)

Prefer concepts over SFINAE:

```cpp
// ✅ Correct: use concept
template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
struct Vector3 { };

// ❌ Avoid: complex SFINAE
template <typename T, 
          typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Vector3 { };
```

### Template Parameters

```cpp
// Single type: T
template <typename T>
class Container { };

// Multiple types: descriptive names
template <typename Key, typename Value>
class Map { };

// Concepts: concept name
template <Arithmetic T>
struct Vector3 { };
```

### Requires Clause

```cpp
// Preferred style
template <typename T>
[[nodiscard]] constexpr T Determinant() const
  requires(M == N)
{
  // ...
}
```

---

## Comments

### Comment Style

- Use `//` for comments (preferred)
- `/* */` acceptable for multi-line comments

### File Comments

```cpp
// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// [license text...]
```

### Class Comments

```cpp
// Iterates over the contents of a GargantuanTable.
//
// Example:
//   std::unique_ptr<Iterator> iter = table->NewIterator();
//   for (iter->Seek("foo"); !iter->done(); iter->Next()) {
//     process(iter->key(), iter->value());
//   }
class GargantuanTableIterator {
  // ...
};
```

### TODO Comments

```cpp
// TODO: bug 12345678 - Remove this after the 2047q4 compatibility window.
// TODO(John): Use a "*" here for concatenation operator.
```

---

## C++23 Best Practices

### Use Modern Features

```cpp
// std::format instead of << for formatting
std::string message = std::format("Value: {}", value);

// std::expected for error handling
std::expected<int, Error> ParseInt(std::string_view str);

// Structured bindings
auto [iter, success] = map.insert({key, value});

// Range-based for with auto
for (const auto& item : container) {
  Process(item);
}
```

### Avoid

```cpp
// ❌ Avoid using namespace in headers
using namespace std;

// ❌ Avoid macros when possible
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// ✅ Prefer
template <typename T>
constexpr T Max(T a, T b) { return (a > b) ? a : b; }

// ❌ Avoid new/delete
int* ptr = new int[100];
delete[] ptr;

// ✅ Prefer smart pointers
auto ptr = std::make_unique<int[]>(100);
```

### noexcept

Use `noexcept` for functions that cannot throw exceptions:

```cpp
// constexpr math operations
[[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const
    noexcept;

// Move constructors
Vector3(Vector3&&) noexcept = default;
Vector3& operator=(Vector3&&) noexcept = default;
```

---

## Tools

### clang-format

Automatically format code:

```bash
clang-format -i --style=file src/MyFile.cpp
```

### Style Verification

The `.clang-format` file at the project root ensures automatic consistency.

---

## Resources

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [cppreference.com](https://en.cppreference.com/)
