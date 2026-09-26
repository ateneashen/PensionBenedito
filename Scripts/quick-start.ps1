# =============================================================================
# Script de Inicio Rápido
# =============================================================================
# Muestra un menú con las acciones más comunes
# Uso: .\Scripts\quick-start.ps1
# =============================================================================

$ErrorActionPreference = "Stop"

function Show-Menu {
    Clear-Host
    Write-Host ""
    Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║     🎮 PENSION BENEDITO - MENÚ RÁPIDO                   ║" -ForegroundColor Cyan
    Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  📁 Estado del Proyecto" -ForegroundColor Yellow
    Write-Host "  ────────────────────────────────────────────────────────"
    Write-Host "  1. Ver estado de Git"
    Write-Host "  2. Ver historial de commits"
    Write-Host "  3. Ver ramas"
    Write-Host ""
    Write-Host "  💾 Guardar Cambios" -ForegroundColor Yellow
    Write-Host "  ────────────────────────────────────────────────────────"
    Write-Host "  4. Commit rápido (todos los archivos)"
    Write-Host "  5. Commit personalizado"
    Write-Host "  6. Push a GitHub"
    Write-Host ""
    Write-Host "  🏷️  Versiones" -ForegroundColor Yellow
    Write-Host "  ────────────────────────────────────────────────────────"
    Write-Host "  7. Crear backup (tag)"
    Write-Host "  8. Ver todos los tags"
    Write-Host ""
    Write-Host "  🔧 Utilidades" -ForegroundColor Yellow
    Write-Host "  ────────────────────────────────────────────────────────"
    Write-Host "  9. Abrir GitHub en navegador"
    Write-Host "  10. Ver diferencias (diff)"
    Write-Host "  11. Limpiar archivos no rastreados"
    Write-Host ""
    Write-Host "  ❓ Ayuda" -ForegroundColor Yellow
    Write-Host "  ────────────────────────────────────────────────────────"
    Write-Host "  12. Guía de GitHub"
    Write-Host "  13. Comandos útiles"
    Write-Host ""
    Write-Host "  0. Salir" -ForegroundColor Red
    Write-Host ""
}

function Show-GitStatus {
    Write-Host ""
    Write-Host "📊 Estado de Git:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    git status
    Write-Host ""
}

function Show-GitLog {
    Write-Host ""
    Write-Host "📜 Historial de Commits (últimos 10):" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    git log --oneline -10
    Write-Host ""
}

function Show-GitBranches {
    Write-Host ""
    Write-Host "🌿 Ramas:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    git branch -a
    Write-Host ""
}

function Quick-Commit {
    Write-Host ""
    Write-Host "💾 Commit Rápido:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    
    $message = Read-Host "Mensaje del commit (o Enter para cancelar)"
    
    if ($message) {
        git add .
        git commit -m "chore: $message"
        Write-Host ""
        Write-Host "✅ Commit creado" -ForegroundColor Green
    }
    Write-Host ""
}

function Custom-Commit {
    Write-Host ""
    Write-Host "💾 Commit Personalizado:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    Write-Host ""
    Write-Host "Tipos de commit:" -ForegroundColor Yellow
    Write-Host "  feat:     Nueva característica"
    Write-Host "  fix:      Corrección de bug"
    Write-Host "  docs:     Documentación"
    Write-Host "  style:    Formato"
    Write-Host "  refactor: Reestructurar código"
    Write-Host "  test:     Tests"
    Write-Host "  chore:    Mantenimiento"
    Write-Host ""
    
    $type = Read-Host "Tipo (feat/fix/docs/etc)"
    $message = Read-Host "Mensaje"
    
    if ($type -and $message) {
        git add .
        git commit -m "${type}: ${message}"
        Write-Host ""
        Write-Host "✅ Commit creado" -ForegroundColor Green
    }
    Write-Host ""
}

function Push-ToGitHub {
    Write-Host ""
    Write-Host "🚀 Push a GitHub:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    
    $branch = git branch --show-current
    Write-Host "Rama actual: $branch" -ForegroundColor Gray
    
    git push origin $branch
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "✅ Cambios subidos" -ForegroundColor Green
    }
    Write-Host ""
}

function Create-Backup {
    Write-Host ""
    Write-Host "🏷️  Crear Backup:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    
    $version = Read-Host "Versión (ej: v1.0.0)"
    $message = Read-Host "Descripción"
    
    if ($version -and $message) {
        git tag -a $version -m $message
        Write-Host ""
        Write-Host "✅ Tag $version creado" -ForegroundColor Green
        
        $push = Read-Host "¿Subir a GitHub? (s/n)"
        if ($push -eq "s" -or $push -eq "S") {
            git push origin $version
            Write-Host "✅ Tag subido" -ForegroundColor Green
        }
    }
    Write-Host ""
}

function Show-Tags {
    Write-Host ""
    Write-Host "🏷️  Tags:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    git tag -l
    Write-Host ""
}

function Open-GitHub {
    Write-Host ""
    Write-Host "🌐 Abriendo GitHub..." -ForegroundColor Cyan
    
    $remoteUrl = git remote get-url origin 2>&1
    if ($remoteUrl -match "github.com") {
        $repoUrl = $remoteUrl -replace "\.git$", ""
        Start-Process $repoUrl
        Write-Host "✅ Abierto en navegador" -ForegroundColor Green
    } else {
        Write-Host "❌ No se encontró URL de GitHub" -ForegroundColor Red
    }
    Write-Host ""
}

function Show-Diff {
    Write-Host ""
    Write-Host "📝 Diferencias:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    git diff --stat
    Write-Host ""
}

function Clean-Untracked {
    Write-Host ""
    Write-Host "🧹 Limpiando archivos no rastreados..." -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    
    $files = git clean -n
    if ($files) {
        Write-Host "Archivos a eliminar:" -ForegroundColor Yellow
        $files
        Write-Host ""
        
        $confirm = Read-Host "¿Eliminar estos archivos? (s/n)"
        if ($confirm -eq "s" -or $confirm -eq "S") {
            git clean -f
            Write-Host "✅ Archivos eliminados" -ForegroundColor Green
        }
    } else {
        Write-Host "No hay archivos no rastreados" -ForegroundColor Gray
    }
    Write-Host ""
}

function Show-Guide {
    Write-Host ""
    Write-Host "📚 Guía de GitHub:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    Write-Host ""
    Write-Host "Lee el archivo GITHUB_SETUP.md para una guía completa" -ForegroundColor White
    Write-Host ""
    Write-Host "Comandos básicos:" -ForegroundColor Yellow
    Write-Host "  git status          - Ver estado"
    Write-Host "  git add .           - Agregar todos los cambios"
    Write-Host "  git commit -m 'msg' - Crear commit"
    Write-Host "  git push            - Subir a GitHub"
    Write-Host "  git pull            - Bajar de GitHub"
    Write-Host "  git log             - Ver historial"
    Write-Host "  git branch          - Ver ramas"
    Write-Host ""
}

function Show-UsefulCommands {
    Write-Host ""
    Write-Host "🔧 Comandos Útiles:" -ForegroundColor Cyan
    Write-Host "────────────────────────────────────────────────────────"
    Write-Host ""
    Write-Host "Diario:" -ForegroundColor Yellow
    Write-Host "  .\Scripts\commit.ps1 -Type 'feat' -Message 'desc'"
    Write-Host "  .\Scripts\push.ps1"
    Write-Host "  .\Scripts\backup.ps1 -Version 'v1.0.0'"
    Write-Host ""
    Write-Host "Git:" -ForegroundColor Yellow
    Write-Host "  git stash           - Guardar cambios temporalmente"
    Write-Host "  git stash pop       - Recuperar cambios guardados"
    Write-Host "  git reset HEAD~1    - Deshacer último commit"
    Write-Host "  git checkout .      - Descartar todos los cambios"
    Write-Host ""
    Write-Host "GitHub:" -ForegroundColor Yellow
    Write-Host "  gh repo view        - Ver repositorio"
    Write-Host "  gh issue list       - Ver issues"
    Write-Host "  gh pr list          - Ver pull requests"
    Write-Host ""
}

# Main loop
do {
    Show-Menu
    $choice = Read-Host "Selecciona una opción"
    
    switch ($choice) {
        "1" { Show-GitStatus }
        "2" { Show-GitLog }
        "3" { Show-GitBranches }
        "4" { Quick-Commit }
        "5" { Custom-Commit }
        "6" { Push-ToGitHub }
        "7" { Create-Backup }
        "8" { Show-Tags }
        "9" { Open-GitHub }
        "10" { Show-Diff }
        "11" { Clean-Untracked }
        "12" { Show-Guide }
        "13" { Show-UsefulCommands }
        "0" { 
            Write-Host ""
            Write-Host "👋 ¡Hasta luego!" -ForegroundColor Green
            Write-Host ""
            break 
        }
        default {
            Write-Host ""
            Write-Host "❌ Opción no válida" -ForegroundColor Red
            Start-Sleep -Seconds 1
        }
    }
    
    if ($choice -ne "0") {
        Read-Host "Presiona Enter para continuar"
    }
} while ($choice -ne "0")