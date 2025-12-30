# Script de test du build ImEngine
# Usage: .\test-build.ps1 [preset]

param(
    [string]$Preset = "editor-app"
)

$ErrorActionPreference = "Stop"

Write-Host "=== ImEngine Build Test ===" -ForegroundColor Cyan
Write-Host "Preset: $Preset" -ForegroundColor Yellow
Write-Host ""

# Nettoyer le dossier de build
$BuildDir = "out\build\$Preset"
if (Test-Path $BuildDir) {
    Write-Host "[1/3] Cleaning build directory..." -ForegroundColor Green
    Remove-Item -Recurse -Force $BuildDir
} else {
    Write-Host "[1/3] Build directory doesn't exist, skipping clean" -ForegroundColor Yellow
}

# Configuration
Write-Host "[2/3] Configuring with CMake..." -ForegroundColor Green
cmake --preset $Preset
if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "[3/3] Building..." -ForegroundColor Green
cmake --build --preset "$Preset-Debug"
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "=== Build Successful ===" -ForegroundColor Green
Write-Host "Binaries are in: $BuildDir\bin\Debug\RUNTIME\" -ForegroundColor Cyan

