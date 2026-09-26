# =============================================================================
# Script de Commit Rápido
# =============================================================================
# Facilita hacer commits con mensajes significativos
# Uso: .\Scripts\commit.ps1 -Type "feat" -Message "Añadir sistema de diálogos"
# =============================================================================

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("feat", "fix", "docs", "style", "refactor", "test", "chore")]
    [string]$Type,
    
    [Parameter(Mandatory=$true)]
    [string]$Message,
    
    [switch]$All,
    [switch]$Push
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "📝 Preparando commit..." -ForegroundColor Cyan
Write-Host ""

# Show current status
Write-Host "📊 Estado actual:" -ForegroundColor Yellow
git status --short
Write-Host ""

# Add files
if ($All) {
    Write-Host "➕ Agregando todos los archivos..." -ForegroundColor Yellow
    git add .
} else {
    Write-Host "➕ Agregando archivos modificados..." -ForegroundColor Yellow
    git add -u
}

# Show what will be committed
Write-Host ""
Write-Host "📋 Archivos a commitear:" -ForegroundColor Yellow
git diff --cached --name-only
Write-Host ""

# Create commit message
$commitMessage = "${Type}: ${Message}"

# Confirm
Write-Host "💬 Mensaje del commit:" -ForegroundColor Yellow
Write-Host "   $commitMessage" -ForegroundColor White
Write-Host ""

$confirm = Read-Host "¿Confirmar commit? (s/n)"
if ($confirm -ne "s" -and $confirm -ne "S") {
    Write-Host "❌ Commit cancelado" -ForegroundColor Red
    exit 0
}

# Make commit
Write-Host ""
Write-Host "💾 Creando commit..." -ForegroundColor Yellow
git commit -m $commitMessage

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "✅ Commit creado exitosamente" -ForegroundColor Green
    Write-Host ""
    
    # Show commit info
    git log -1 --oneline
    Write-Host ""
    
    # Push if requested
    if ($Push) {
        Write-Host "🚀 Subiendo a GitHub..." -ForegroundColor Yellow
        git push
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Cambios subidos a GitHub" -ForegroundColor Green
        } else {
            Write-Host "⚠️  Error al subir. Intenta: git push" -ForegroundColor Yellow
        }
    }
} else {
    Write-Host "❌ Error al crear commit" -ForegroundColor Red
}

Write-Host ""