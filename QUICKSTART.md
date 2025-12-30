# Quick Start - Verification rapide

## Test en 3 commandes

```powershell
# 1. Nettoyer (optionnel)
Remove-Item -Recurse -Force "out\build\editor-app" -ErrorAction SilentlyContinue

# 2. Configurer
cmake --preset editor-app

# 3. Compiler
cmake --build --preset editor-Debug
```

## Vérification de succès

Si vous voyez à la fin :
```
[XX/XX] Linking CXX executable ...ImEngine-Editor.exe
```

Et aucune erreur, **c'est gagné ! ✅**

## En cas de problème

1. Vérifier les prérequis :
```powershell
cmake --version        # Doit être >= 3.29
clang-cl --version     # Ou cl --version pour MSVC
ninja --version
git --version
```

2. Nettoyer complètement :
```powershell
Remove-Item -Recurse -Force out\
cmake --preset editor-app
```

3. Consulter les docs :
- `docs/BUILD_VALIDATION.md` pour validation détaillée
- `docs/VULKAN_MASM_FIX.md` pour problèmes Vulkan
- `ARCHITECTURE.md` pour comprendre la structure

## Lancer l'application

```powershell
.\out\build\editor-app\bin\Debug\RUNTIME\ImEngine-Editor.exe
```

---

**Temps total estimé : 3-5 minutes** (première fois avec téléchargements)

