# =============================================================================
# Script de Backup con Tags
# =============================================================================
# Crea un backup etiquetado del proyecto
# Uso: .\Scripts\backup.ps1 -Version "v1.0.0" -Message "Primera versión funcional"
# =============================================================================

param(
    [Parameter(Mandatory=$true)]
    [string]$Version,
    
    [string]$Message = "Release $Version",
    
    [switch]$Push
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "💾 Creando backup etiquetado..." -ForegroundColor Cyan
Write-Host ""

# Validate version format
if ($Version -notmatch "^v\d+\.\d+\.\d+$") {
    Write-Host "⚠️  Formato de versión recomendado: vX.Y.Z" -ForegroundColor Yellow
    Write-Host "   Ejemplo: v1.0.0, v0.2.1, v2.0.0" -ForegroundColor Gray
    Write-Host ""
    $continue = Read-Host "¿Continuar con '$Version'? (s/n)"
    if ($continue -ne "s" -and $continue -ne "S") {
        Write-Host "❌ Backup cancelado" -ForegroundColor Red
        exit 0
    }
}

# Check for uncommitted changes
$status = git status --porcelain
if ($status) {
    Write-Host "⚠️  Hay cambios sin commitear:" -ForegroundColor Yellow
    git status --short
    Write-Host ""
    
    $commit = Read-Host "¿Deseas commitear estos cambios antes del backup? (s/n)"
    if ($commit -eq "s" -or $commit -eq "S") {
        git add .
        git commit -m "chore: Pre-release commit for $Version"
        Write-Host ""
    }
}

# Show current state
Write-Host "📊 Estado actual del proyecto:" -ForegroundColor Yellow
Write-Host "   Rama: $(git branch --show-current)" -ForegroundColor Gray
Write-Host "   Último commit: $(git log -1 --oneline)" -ForegroundColor Gray
Write-Host "   Archivos modificados: $(git status --short | Measure-Object | Select-Object -ExpandProperty Count)" -ForegroundColor Gray
Write-Host ""

# Create annotated tag
Write-Host "🏷️  Creando tag $Version..." -ForegroundColor Yellow
git tag -a $Version -m $Message

if ($LASTEXITCODE -eq 0) {
    Write-Host "   ✅ Tag creado exitosamente" -ForegroundColor Green
    Write-Host ""
    
    # Show tag info
    Write-Host "📋 Información del tag:" -ForegroundColor Yellow
    git show $Version --no-patch
    Write-Host ""
    
    # Push if requested
    if ($Push) {
        Write-Host "📤 Subiendo tag a GitHub..." -ForegroundColor Yellow
        git push origin $Version
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "   ✅ Tag subido a GitHub" -ForegroundColor Green
            
            # Show release URL
            $remoteUrl = git remote get-url origin 2>&1
            if ($remoteUrl -match "github.com") {
                $repoUrl = $remoteUrl -replace "\.git$", ""
                Write-Host ""
                Write-Host "🔗 Crear release en GitHub:" -ForegroundColor Cyan
                Write-Host "   $repoUrl/releases/new?tag=$Version" -ForegroundColor White
            }
        } else {
            Write-Host "   ⚠️  Error al subir tag" -ForegroundColor Yellow
        }
    }
    
    # List all tags
    Write-Host ""
    Write-Host "📦 Todos los tags:" -ForegroundColor Yellow
    git tag -l
} else {
    Write-Host "   ❌ Error al crear tag" -ForegroundColor Red
}

Write-Host ""