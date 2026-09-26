# =============================================================================
# Script para Descargar Repositorios de GAS
# =============================================================================
# Descarga repositorios educativos de Gameplay Ability System
# Uso: .\repo_assets\download-gas-repos.ps1
# =============================================================================

param(
    [switch]$Force,
    [switch]$SkipExisting
)

$ErrorActionPreference = "Stop"

$basePath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\repo_assets"
$repos = @(
    @{
        Name = "ActionRPG"
        URL = "https://github.com/ProjectBorealis/ActionRPG.git"
        Description = "Epic's official Action RPG sample with GAS"
        TargetPath = "$basePath\Examples\ActionRPG"
        License = "MIT"
    },
    @{
        Name = "GASDocumentation"
        URL = "https://github.com/tranek/GASDocumentation.git"
        Description = "Comprehensive GAS documentation and examples"
        TargetPath = "$basePath\GAS\Documentation\GASDocumentation"
        License = "MIT"
    },
    @{
        Name = "GASShooter"
        URL = "https://github.com/tranek/GASShooter.git"
        Description = "GAS shooter example project"
        TargetPath = "$basePath\Examples\GASShooter"
        License = "MIT"
    },
    @{
        Name = "LyraStarterGame"
        URL = "https://github.com/EpicGames/UnrealEngine.git"
        Description = "Lyra starter game (requires Epic access)"
        TargetPath = "$basePath\Examples\Lyra"
        License = "Epic License"
        Skip = $true
    }
)

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     DESCARGA DE REPOSITORIOS GAS                             " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

$totalRepos = $repos.Count
$downloaded = 0
$skipped = 0
$errors = 0

foreach ($repo in $repos) {
    $repoNum = $repos.IndexOf($repo) + 1
    Write-Host "[$repoNum/$totalRepos] $($repo.Name)" -ForegroundColor Yellow
    Write-Host "   Descripcion: $($repo.Description)" -ForegroundColor Gray
    Write-Host "   URL: $($repo.URL)" -ForegroundColor Gray
    Write-Host "   Licencia: $($repo.License)" -ForegroundColor Gray
    Write-Host ""
    
    # Check if should skip
    if ($repo.Skip) {
        Write-Host "   [SKIP] Requiere acceso especial de Epic" -ForegroundColor Yellow
        $skipped++
        Write-Host ""
        continue
    }
    
    # Check if already exists
    if (Test-Path $repo.TargetPath) {
        if ($SkipExisting) {
            Write-Host "   [SKIP] Ya existe" -ForegroundColor Yellow
            $skipped++
            Write-Host ""
            continue
        }
        
        if (-not $Force) {
            $overwrite = Read-Host "   Ya existe. ¿Sobrescribir? (s/n)"
            if ($overwrite -ne "s" -and $overwrite -ne "S") {
                Write-Host "   [SKIP] Omitido por usuario" -ForegroundColor Yellow
                $skipped++
                Write-Host ""
                continue
            }
        }
        
        # Remove existing
        Write-Host "   Eliminando version existente..." -ForegroundColor Gray
        Remove-Item -Recurse -Force $repo.TargetPath
    }
    
    # Clone repository
    Write-Host "   Clonando repositorio..." -ForegroundColor Gray
    try {
        git clone --depth 1 $repo.URL $repo.TargetPath 2>&1 | Out-Null
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "   [OK] Descargado exitosamente" -ForegroundColor Green
            
            # Create README
            $readmePath = "$($repo.TargetPath)\README_REPO_ASSETS.md"
            $readmeContent = @"
# $($repo.Name)

**Descripcion**: $($repo.Description)
**URL**: $($repo.URL)
**Licencia**: $($repo.License)
**Descargado**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")

## Contenido

Este repositorio contiene ejemplos y referencia para el Gameplay Ability System.

## Como Usar

1. Revisa el codigo fuente para aprender patrones
2. Copia las clases que necesites a nuestro proyecto
3. Adapta el codigo a nuestras necesidades
4. Documenta los cambios

## Notas

- Compatible con UE4.27 (verificar en cada clase)
- Revisar la licencia antes de usar en produccion
- Adaptar a NarrativeActionKit y Pension Benedito
"@
            $readmeContent | Out-File -FilePath $readmePath -Encoding UTF8
            
            $downloaded++
        } else {
            Write-Host "   [ERROR] Error al clonar" -ForegroundColor Red
            $errors++
        }
    } catch {
        Write-Host "   [ERROR] $($_.Exception.Message)" -ForegroundColor Red
        $errors++
    }
    
    Write-Host ""
}

# Summary
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     RESUMEN                                                  " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "  Repositorios totales:    $totalRepos" -ForegroundColor White
Write-Host "  Descargados:             $downloaded" -ForegroundColor Green
Write-Host "  Omitidos:                $skipped" -ForegroundColor Yellow
Write-Host "  Errores:                 $errors" -ForegroundColor Red
Write-Host ""

if ($downloaded -gt 0) {
    Write-Host "  Proximo paso:" -ForegroundColor Yellow
    Write-Host "  Revisa el codigo descargado en:" -ForegroundColor White
    Write-Host "  $basePath" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  Lee los README en cada carpeta para entender el contenido." -ForegroundColor White
}

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""