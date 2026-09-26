# =============================================================================
# Script de Configuracion Inicial para GitHub
# =============================================================================
# Este script configura todo lo necesario para empezar a usar GitHub
# Ejecutar UNA SOLA VEZ al inicio del proyecto
# =============================================================================

param(
    [Parameter(Mandatory=$true)]
    [string]$UserName,
    
    [Parameter(Mandatory=$true)]
    [string]$UserEmail,
    
    [string]$RepoName = "PensionBenedito",
    [string]$Description = "Horror mystery game set in 1930s Spain with reusable narrative framework"
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     CONFIGURACION INICIAL DE GITHUB                           " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Configure Git identity
Write-Host "[PASO 1] Configurando identidad de Git..." -ForegroundColor Yellow
git config --global user.name $UserName
git config --global user.email $UserEmail
git config --global pull.rebase false
git config --global core.autocrlf true
Write-Host "   [OK] Identidad configurada: $UserName <$UserEmail>" -ForegroundColor Green
Write-Host ""

# Step 2: Initialize Git repository
Write-Host "[PASO 2] Inicializando repositorio Git..." -ForegroundColor Yellow
$projectPath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"

if (Test-Path "$projectPath\.git") {
    Write-Host "   [!] Repositorio Git ya existe" -ForegroundColor Yellow
} else {
    Set-Location $projectPath
    git init
    Write-Host "   [OK] Repositorio Git inicializado" -ForegroundColor Green
}
Write-Host ""

# Step 3: Configure Git LFS for large files
Write-Host "[PASO 3] Configurando Git LFS para archivos grandes..." -ForegroundColor Yellow
git lfs install
git lfs track "*.uasset"
git lfs track "*.umap"
Write-Host "   [OK] Git LFS configurado para archivos .uasset y .umap" -ForegroundColor Green
Write-Host ""

# Step 4: Create initial commit
Write-Host "[PASO 4] Creando commit inicial..." -ForegroundColor Yellow
git add .
git commit -m "feat: Initial project setup with NarrativeActionKit framework

- NarrativeActionKit: Reusable framework for narrative action games
- Pension Benedito: Horror mystery game in 1930s Spain
- LocalLLM TestBench: Research system using local AI model
- Complete documentation and automation scripts"
Write-Host "   [OK] Commit inicial creado" -ForegroundColor Green
Write-Host ""

# Step 5: Authenticate with GitHub
Write-Host "[PASO 5] Autenticacion con GitHub..." -ForegroundColor Yellow
Write-Host "   Se abrira tu navegador para iniciar sesion." -ForegroundColor Gray
Write-Host "   Sigue las instrucciones en pantalla." -ForegroundColor Gray
Write-Host ""

$authStatus = gh auth status 2>&1
if ($authStatus -match "Logged in") {
    Write-Host "   [OK] Ya estas autenticado en GitHub" -ForegroundColor Green
} else {
    Write-Host "   Iniciando proceso de autenticacion..." -ForegroundColor Gray
    gh auth login
}
Write-Host ""

# Step 6: Create GitHub repository
Write-Host "[PASO 6] Creando repositorio en GitHub..." -ForegroundColor Yellow

$existingRepo = gh repo view $RepoName 2>&1
if ($existingRepo -match "Could not resolve") {
    gh repo create $RepoName --public --description $Description --source=. --push
    Write-Host "   [OK] Repositorio creado y codigo subido" -ForegroundColor Green
} else {
    Write-Host "   [!] El repositorio $RepoName ya existe en GitHub" -ForegroundColor Yellow
    $push = Read-Host "   Deseas subir el codigo? (s/n)"
    if ($push -eq "s" -or $push -eq "S") {
        git push -u origin main
        Write-Host "   [OK] Codigo subido a GitHub" -ForegroundColor Green
    }
}
Write-Host ""

# Step 7: Verify setup
Write-Host "[PASO 7] Verificando configuracion..." -ForegroundColor Yellow
Write-Host ""
Write-Host "   Configuracion de Git:" -ForegroundColor Gray
Write-Host "   - Usuario: $(git config --global user.name)" -ForegroundColor Gray
Write-Host "   - Email: $(git config --global user.email)" -ForegroundColor Gray
Write-Host ""
Write-Host "   Repositorio:" -ForegroundColor Gray
Write-Host "   - Local: $(git rev-parse --show-toplevel)" -ForegroundColor Gray
$remoteUrl = git remote get-url origin 2>&1
if ($remoteUrl -match "https") {
    Write-Host "   - Remoto: $remoteUrl" -ForegroundColor Gray
}
Write-Host ""
Write-Host "   Git LFS:" -ForegroundColor Gray
git lfs track
Write-Host ""

# Final message
Write-Host "================================================================" -ForegroundColor Green
Write-Host "     CONFIGURACION COMPLETADA EXITOSAMENTE!                    " -ForegroundColor Green
Write-Host "================================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Proximos pasos:" -ForegroundColor Yellow
Write-Host "   1. Lee GITHUB_SETUP.md para aprender el flujo diario" -ForegroundColor White
Write-Host "   2. Usa .\Scripts\commit.ps1 para hacer commits rapidos" -ForegroundColor White
Write-Host "   3. Usa .\Scripts\push.ps1 para subir cambios a GitHub" -ForegroundColor White
Write-Host ""

try {
    $ghUser = gh api user --jq .login 2>&1
    if ($ghUser -match "^[a-zA-Z0-9]") {
        Write-Host "Tu repositorio: https://github.com/$ghUser/$RepoName" -ForegroundColor Cyan
    }
} catch {
    Write-Host "Tu repositorio: https://github.com/TU_USUARIO/$RepoName" -ForegroundColor Cyan
}
Write-Host ""