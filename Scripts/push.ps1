# =============================================================================
# Script de Push a GitHub
# =============================================================================
# Sube los cambios locales a GitHub de forma segura
# Uso: .\Scripts\push.ps1
# =============================================================================

param(
    [switch]$Force,
    [switch]$Tags
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "🚀 Preparando push a GitHub..." -ForegroundColor Cyan
Write-Host ""

# Check for uncommitted changes
$status = git status --porcelain
if ($status) {
    Write-Host "⚠️  Hay cambios sin commitear:" -ForegroundColor Yellow
    git status --short
    Write-Host ""
    
    $commit = Read-Host "¿Deseas commitear estos cambios primero? (s/n)"
    if ($commit -eq "s" -or $commit -eq "S") {
        $message = Read-Host "Mensaje del commit"
        git add .
        git commit -m "chore: $message"
        Write-Host ""
    }
}

# Get current branch
$currentBranch = git branch --show-current
Write-Host "📍 Rama actual: $currentBranch" -ForegroundColor Gray
Write-Host ""

# Pull latest changes first (safe practice)
Write-Host "📥 Obteniendo últimos cambios remotos..." -ForegroundColor Yellow
git fetch origin

# Check if we're behind
$status = git status -sb
if ($status -match "behind") {
    Write-Host "⚠️  Tu rama está detrás de la remota" -ForegroundColor Yellow
    $pull = Read-Host "¿Deseas hacer pull primero? (s/n)"
    if ($pull -eq "s" -or $pull -eq "S") {
        git pull --rebase
        Write-Host ""
    }
}

# Push
Write-Host "📤 Subiendo cambios a GitHub..." -ForegroundColor Yellow

if ($Force) {
    Write-Host "⚠️  Usando --force (¡cuidado!)" -ForegroundColor Red
    git push --force-with-lease origin $currentBranch
} else {
    git push origin $currentBranch
}

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "✅ Cambios subidos exitosamente" -ForegroundColor Green
    
    # Push tags if requested
    if ($Tags) {
        Write-Host ""
        Write-Host "🏷️  Subiendo tags..." -ForegroundColor Yellow
        git push --tags
        Write-Host "✅ Tags subidos" -ForegroundColor Green
    }
    
    # Show remote URL
    $remoteUrl = git remote get-url origin 2>&1
    if ($remoteUrl -match "github.com") {
        $repoUrl = $remoteUrl -replace "\.git$", ""
        Write-Host ""
        Write-Host "🔗 Ver en GitHub: $repoUrl" -ForegroundColor Cyan
    }
} else {
    Write-Host ""
    Write-Host "❌ Error al subir cambios" -ForegroundColor Red
    Write-Host ""
    Write-Host "💡 Posibles soluciones:" -ForegroundColor Yellow
    Write-Host "   1. Verifica tu conexión a internet" -ForegroundColor White
    Write-Host "   2. Ejecuta: gh auth status" -ForegroundColor White
    Write-Host "   3. Si hay conflictos, ejecuta: git pull --rebase" -ForegroundColor White
}

Write-Host ""