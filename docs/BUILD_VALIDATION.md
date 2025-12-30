# Guide de Validation du Build

Ce document liste les étapes pour valider que l'architecture modernisée fonctionne correctement.

## ✅ Checklist de Validation

### 1. Configuration CMake

```powershell
# Nettoyer si nécessaire
Remove-Item -Recurse -Force "out\build\editor-app" -ErrorAction SilentlyContinue

# Configurer
cmake --preset editor-app
```

**Vérifications attendues :**
- ✅ Message "Using ccache" apparaît (ou "Downloaded ccache" si première fois)
- ✅ Message "Vulkan SDK not found. Fetching..." si SDK non installé
- ✅ Aucun warning "FetchContent_Populate() is deprecated"
- ✅ Aucun warning "Could not find working assembler" (ou si présent, n'empêche pas le build)
- ✅ Message final "Configuring done" et "Generating done"

### 2. Compilation

```powershell
cmake --build --preset editor-Debug --target ImEngine-Editor
```

**Vérifications attendues :**
- ✅ Compilation de GLFW (si première fois)
- ✅ Compilation de Vulkan-Loader (si SDK non installé)
- ✅ Compilation de ImGui
- ✅ Compilation de ImEngine-Core
- ✅ Compilation de ImEngine-Engine
- ✅ Compilation de ImEngine-Editor
- ✅ **AUCUNE erreur "ml64: CreateProcess failed"**
- ✅ **AUCUNE erreur liée à l'assembleur MASM**
- ✅ Linking réussi

### 3. Vérification des Binaires

```powershell
# Vérifier que les fichiers existent
Test-Path "out\build\editor-app\bin\Debug\RUNTIME\ImEngine-Core.dll"
Test-Path "out\build\editor-app\bin\Debug\RUNTIME\ImEngine-Engine.dll"
Test-Path "out\build\editor-app\bin\Debug\RUNTIME\ImEngine-Editor.exe"
```

**Tous doivent retourner `True`**

### 4. Test d'Exécution (Optionnel)

```powershell
# Lancer l'éditeur
.\out\build\editor-app\bin\Debug\RUNTIME\ImEngine-Editor.exe
```

**Vérifications attendues :**
- ✅ L'application démarre sans crash
- ✅ Une fenêtre GLFW s'ouvre
- ✅ Le contexte Vulkan s'initialise correctement

### 5. Build Incrémental

```powershell
# Modifier un fichier source (ex: ajouter un commentaire dans Engine.cpp)
# Puis recompiler
cmake --build --preset editor-Debug --target ImEngine-Editor
```

**Vérifications attendues :**
- ✅ Build très rapide (< 30 secondes)
- ✅ Seuls les fichiers modifiés sont recompilés
- ✅ ccache accélère le processus

## 🔍 Résolution de Problèmes

### Erreur "ml64: CreateProcess failed"

**Cause :** `BUILD_UNKNOWN_FUNCTIONS` n'est pas correctement désactivé.

**Solution :**
```powershell
# Nettoyer complètement
Remove-Item -Recurse -Force "out\build\editor-app"
# Reconfigurer
cmake --preset editor-app
```

Vérifier dans la sortie CMake que `BUILD_UNKNOWN_FUNCTIONS OFF` est bien appliqué.

### Erreur "Vulkan::Vulkan target not found"

**Cause :** Problème de téléchargement de Vulkan-Loader.

**Solution :**
```powershell
# Nettoyer le cache FetchContent
Remove-Item -Recurse -Force "out\build\editor-app\_deps\vulkan-*"
# Reconfigurer
cmake --preset editor-app
```

### ccache n'est pas utilisé

**Cause :** ccache n'a pas été détecté ou téléchargé.

**Vérification :**
```powershell
# Vérifier la présence
Test-Path "out\build\editor-app\tools\ccache\ccache-4.10.2-windows-x86_64\ccache.exe"
```

Si `False`, supprimer `out/build/editor-app` et reconfigurer.

### Compilation très lente

**Causes possibles :**
1. Première compilation (normal, 2-5 min)
2. ccache pas actif
3. Ninja pas utilisé

**Solution :**
```powershell
# Vérifier que Ninja est le générateur
cmake --preset editor-app | Select-String "Ninja"
# Devrait afficher : -- Generator: Ninja Multi-Config
```

## 📊 Métriques de Performance Attendues

### Temps de Configuration
- **Première fois** : 30-60s (téléchargement ccache + Vulkan)
- **Avec cache** : 5-10s

### Temps de Build
- **Clean build (Debug)** : 2-5 min
- **Incrémental (1 fichier)** : 5-15s
- **Incrémental avec ccache** : 3-10s

### Taille des Binaires (Debug)
- `ImEngine-Core.dll` : ~100-500 KB
- `ImEngine-Engine.dll` : ~200 KB - 1 MB
- `ImEngine-Editor.exe` : ~50-200 KB
- `glfw3.dll` : ~150 KB
- `vulkan-1.dll` : ~600 KB - 1 MB

## ✨ Fonctionnalités Validées

- [x] Téléchargement automatique de ccache
- [x] Téléchargement automatique de Vulkan SDK
- [x] Compilation sans MASM (BUILD_UNKNOWN_FUNCTIONS OFF)
- [x] Compatibilité clang-cl complète
- [x] Flags de sécurité appliqués
- [x] Warnings stricts activés
- [x] Architecture modulaire (Core/Engine/Runtime/Editor)
- [x] Installation SDK fonctionnelle
- [x] CMake presets modernes

## 🎓 Pour Aller Plus Loin

### Tester tous les presets

```powershell
# Engine only
.\test-build.ps1 engine-only

# Sandbox
.\test-build.ps1 sandbox-app

# Editor (déjà testé)
.\test-build.ps1 editor-app
```

### Tester les configurations

```powershell
# RelWithDebInfo
cmake --build --preset editor-RelWithDebInfo

# Release
cmake --build --preset editor-Release
```

### Installation

```powershell
# Installer le SDK
cmake --install out\build\editor-app --prefix out\install\editor-app
```

Vérifier que les headers, libs et DLLs sont copiés dans `out\install\editor-app\`.

---

**Si toutes les vérifications passent, l'architecture est validée et prête pour le développement !** ✅

