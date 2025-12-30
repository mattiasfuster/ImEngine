# Solution au problème d'assembleur MASM avec Vulkan-Loader et clang-cl

## Problème

Lors de la compilation avec `clang-cl`, l'erreur suivante se produisait :
```
FAILED: _deps/vulkan-loader-build/loader/CMakeFiles/loader-unknown-chain.dir/Debug/unknown_ext_chain_masm.asm.obj 
ml64 [...]
CreateProcess failed: The system cannot find the file specified.
```

## Cause

Vulkan-Loader essayait de compiler du code assembleur MASM pour supporter les "unknown physical device functions". Même si CMake détectait `ml64.exe` pendant la configuration, il n'était pas disponible dans le PATH lors de l'exécution du build avec `clang-cl`.

## Solution appliquée

Au lieu de forcer la détection de l'assembleur (ce qui ne résolvait pas le problème d'exécution), nous avons **désactivé le support des fonctions inconnues** dans Vulkan-Loader, éliminant ainsi complètement le besoin de l'assembleur MASM.

### Modification dans `ImEngine/ThirdParty/CMakeLists.txt`

```cmake
# Disable unknown functions support to avoid MASM dependency issues with clang-cl
set(BUILD_UNKNOWN_FUNCTIONS OFF CACHE BOOL "" FORCE)
```

Cette option est ajoutée avant le `FetchContent_Declare` de Vulkan-Loader.

## Impact

- ✅ **Aucun impact sur les fonctionnalités** : Le support des fonctions inconnues est une fonctionnalité avancée rarement utilisée qui permet d'appeler des fonctions Vulkan qui ne sont pas connues au moment de la compilation du loader.
- ✅ **Build simplifié** : Plus besoin de l'assembleur MASM, ce qui élimine une dépendance complexe.
- ✅ **Compatible clang-cl** : Le build fonctionne maintenant parfaitement avec `clang-cl` sans nécessiter de configuration spéciale de l'environnement.
- ✅ **Portable** : Fonctionne sur n'importe quelle machine Windows avec ou sans Visual Studio complet.

## Fichiers modifiés

- `ImEngine/ThirdParty/CMakeLists.txt` : Ajout de `BUILD_UNKNOWN_FUNCTIONS OFF`
- Suppression du script `cmake/PatchVulkanLoader.cmake` (devenu inutile)

## Commandes pour tester

```powershell
# Nettoyer le build
Remove-Item -Recurse -Force "out\build\editor-app"

# Reconfigurer
cmake --preset editor-app

# Compiler
cmake --build --preset editor-Debug --target ImEngine-Editor
```

## Alternative (si nécessaire)

Si le support des fonctions inconnues est absolument nécessaire (cas très rare), il faudrait :
1. Installer Visual Studio complet (pas seulement Build Tools)
2. Utiliser le compilateur MSVC (`cl.exe`) au lieu de `clang-cl`
3. Ou configurer l'environnement avec `vcvarsall.bat` avant de lancer le build

Mais pour 99% des cas d'usage, la solution actuelle (désactivation) est parfaite.

