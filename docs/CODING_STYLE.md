# ImEngine Coding Style Guide

Ce document décrit les conventions de codage d'ImEngine, basées sur le **[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)** adapté pour C++23.

## Table des matières

1. [Conventions de nommage](#conventions-de-nommage)
2. [Formatage](#formatage)
3. [Headers et includes](#headers-et-includes)
4. [Classes et structs](#classes-et-structs)
5. [Fonctions](#fonctions)
6. [Constantes et constexpr](#constantes-et-constexpr)
7. [Templates et concepts](#templates-et-concepts)
8. [Commentaires](#commentaires)
9. [Bonnes pratiques C++23](#bonnes-pratiques-c23)

---

## Conventions de nommage

### Tableau récapitulatif

| Élément | Convention | Exemple |
|---------|------------|---------|
| **Types** (classes, structs, enums) | `PascalCase` | `Vector3`, `LinearAllocator` |
| **Fonctions** | `PascalCase` | `DotProduct()`, `InitWindow()` |
| **Variables locales** | `snake_case` | `extension_count`, `app_info` |
| **Membres de classe** | `snake_case_` (underscore final) | `window_`, `width_` |
| **Membres de struct** | `snake_case` (pas d'underscore) | `x`, `y`, `data` |
| **Constantes** | `kPascalCase` | `kRows`, `kMaxSize` |
| **Paramètres de template** | Majuscule ou `PascalCase` | `T`, `AngleUnit` |
| **Namespaces** | `snake_case` | `imengine::core` |
| **Macros** | `UPPER_SNAKE_CASE` | `IMENGINE_API`, `IM_INFO` |
| **Accesseurs** | `snake_case()` | `window()`, `vulkan_instance()` |

### Types (Classes, Structs, Enums)

```cpp
// ✅ Correct
class Engine { };
struct Vector3 { };
enum class UrlTableError { kOk, kNotFound };

// ❌ Incorrect
class engine { };
struct vector_3 { };
enum class URLTableError { };  // Pas d'acronymes en majuscules
```

### Variables

```cpp
// Variables locales : snake_case
int extension_count = 0;
std::string table_name;

// Membres de classe : snake_case avec underscore final
class MyClass {
 private:
  int count_;
  std::string name_;
};

// Membres de struct : snake_case sans underscore
struct Point {
  float x;
  float y;
};
```

### Constantes

```cpp
// Constantes avec 'k' préfixe
constexpr int kDaysInAWeek = 7;
constexpr size_t kMaxBufferSize = 1024;

// Constantes statiques de classe
class Matrice {
  static constexpr size_t kRows = 4;
  static constexpr size_t kCols = 4;
};
```

### Fonctions et méthodes

```cpp
// Fonctions : PascalCase
void InitWindow();
double DotProduct(const Vector3& a, const Vector3& b);

// Accesseurs : snake_case (style Google moderne)
class Engine {
 public:
  [[nodiscard]] GLFWwindow* window() const { return window_; }
  void set_window(GLFWwindow* window) { window_ = window; }
};
```

---

## Formatage

### Configuration .clang-format

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

- **2 espaces** pour l'indentation
- **Pas de tabulations**
- Modificateurs d'accès indentés d'**1 espace** par rapport à la classe

```cpp
class MyClass {
 public:  // 1 espace
  void Foo();  // 2 espaces
  
 private:
  int bar_;
};
```

### Accolades

Style **Attached** (K&R) : l'accolade ouvrante est sur la même ligne.

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

// ❌ Incorrect (style Allman)
if (condition)
{
  DoSomething();
}
```

### Longueur de ligne

- Maximum **80 caractères** par ligne
- Exceptions : URLs, chemins de fichiers, chaînes de caractères non découpables

### Pointeurs et références

```cpp
// ✅ Correct : aligné à gauche
char* name;
const std::string& ref;
int* GetPointer();

// ❌ Incorrect
char *name;
char * name;
```

---

## Headers et includes

### Ordre des includes

1. Header associé (foo.h pour foo.cc)
2. Ligne vide
3. Headers système C (avec extension `.h`)
4. Ligne vide
5. Headers standard C++
6. Ligne vide
7. Headers de bibliothèques tierces
8. Ligne vide
9. Headers du projet

```cpp
#include "Engine.h"

#include <stdlib.h>

#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#include "Debug/Logger.h"
```

### Include guards

Format : `PROJECT_PATH_FILE_H_`

```cpp
#ifndef IMENGINE_MATHS_OBJECTS_VECTOR_H_
#define IMENGINE_MATHS_OBJECTS_VECTOR_H_

// contenu

#endif  // IMENGINE_MATHS_OBJECTS_VECTOR_H_
```

---

## Classes et structs

### Quand utiliser struct vs class

- **struct** : données passives sans invariants (POD-like)
- **class** : avec invariants, encapsulation, méthodes non triviales

```cpp
// struct : données simples
struct Vector3 {
  float x;
  float y;
  float z;
};

// class : avec invariants et encapsulation
class Engine {
 public:
  void Run();
  
 private:
  GLFWwindow* window_ = nullptr;
};
```

### Ordre de déclaration

```cpp
class MyClass {
 public:
  // 1. Types et aliases
  using ValueType = int;
  
  // 2. Constantes statiques
  static constexpr int kMaxSize = 100;
  
  // 3. Constructeurs et opérateur d'affectation
  MyClass();
  MyClass(const MyClass&) = default;
  MyClass& operator=(const MyClass&) = default;
  
  // 4. Destructeur
  ~MyClass();
  
  // 5. Méthodes
  void DoSomething();
  [[nodiscard]] int value() const { return value_; }
  
 protected:
  // Membres protégés
  
 private:
  // 6. Données membres
  int value_ = 0;
};
```

### Copie et déplacement

Déclarer explicitement les opérations de copie/déplacement :

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

## Fonctions

### Paramètres

```cpp
// Entrées : valeur ou const reference
void Process(int value);
void Process(const std::string& name);

// Sorties : retour par valeur préféré
[[nodiscard]] std::string GetName() const;

// Entrée/Sortie : référence non-const ou pointeur
void Modify(std::string& name);
void Modify(std::string* name);  // si peut être null
```

### Fonctions courtes

- Garder les fonctions courtes et focalisées
- Environ **40 lignes** maximum
- Extraire les sous-tâches en fonctions séparées

### [[nodiscard]]

Utiliser `[[nodiscard]]` pour les fonctions dont la valeur de retour ne doit pas être ignorée :

```cpp
[[nodiscard]] constexpr double SquaredLength() const noexcept;
[[nodiscard]] bool IsValid() const;
[[nodiscard]] std::unique_ptr<Widget> CreateWidget();
```

---

## Constantes et constexpr

### Préférer constexpr à const

```cpp
// ✅ Préféré pour les valeurs connues à la compilation
constexpr int kMaxSize = 1024;
constexpr double kPi = 3.14159265358979323846;

// const pour les valeurs runtime
const int size = ComputeSize();
```

### Fonctions constexpr

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

## Templates et concepts

### Concepts (C++20/23)

Préférer les concepts aux SFINAE :

```cpp
// ✅ Correct : utiliser concept
template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
struct Vector3 { };

// ❌ Éviter : SFINAE complexe
template <typename T, 
          typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Vector3 { };
```

### Paramètres de template

```cpp
// Type unique : T
template <typename T>
class Container { };

// Types multiples : noms descriptifs
template <typename Key, typename Value>
class Map { };

// Concepts : nom du concept
template <Arithmetic T>
struct Vector3 { };
```

### Requires clause

```cpp
// Style préféré
template <typename T>
[[nodiscard]] constexpr T Determinant() const
  requires(M == N)
{
  // ...
}
```

---

## Commentaires

### Style de commentaire

- Utiliser `//` pour les commentaires (préféré)
- `/* */` acceptable pour les commentaires multi-lignes

### Commentaires de fichier

```cpp
// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// [texte de licence...]
```

### Commentaires de classe

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

### Commentaires TODO

```cpp
// TODO: bug 12345678 - Remove this after the 2047q4 compatibility window.
// TODO(John): Use a "*" here for concatenation operator.
```

---

## Bonnes pratiques C++23

### Utiliser les fonctionnalités modernes

```cpp
// std::format au lieu de << pour le formatage
std::string message = std::format("Value: {}", value);

// std::expected pour la gestion d'erreurs
std::expected<int, Error> ParseInt(std::string_view str);

// Structured bindings
auto [iter, success] = map.insert({key, value});

// Range-based for avec auto
for (const auto& item : container) {
  Process(item);
}
```

### Éviter

```cpp
// ❌ Éviter les using namespace dans les headers
using namespace std;

// ❌ Éviter les macros quand possible
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// ✅ Préférer
template <typename T>
constexpr T Max(T a, T b) { return (a > b) ? a : b; }

// ❌ Éviter new/delete
int* ptr = new int[100];
delete[] ptr;

// ✅ Préférer smart pointers
auto ptr = std::make_unique<int[]>(100);
```

### noexcept

Utiliser `noexcept` pour les fonctions qui ne peuvent pas lancer d'exceptions :

```cpp
// Opérations mathématiques constexpr
[[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const
    noexcept;

// Move constructors
Vector3(Vector3&&) noexcept = default;
Vector3& operator=(Vector3&&) noexcept = default;
```

---

## Outils

### clang-format

Formater automatiquement le code :

```bash
clang-format -i --style=file src/MyFile.cpp
```

### Vérification du style

Le fichier `.clang-format` à la racine du projet assure la cohérence automatique.

---

## Ressources

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [cppreference.com](https://en.cppreference.com/)
