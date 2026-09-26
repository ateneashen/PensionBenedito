# =============================================================================
# Script de Ayuda Rapida
# =============================================================================
# Muestra informacion de ayuda y comandos utiles
# Uso: .\Scripts\help.ps1
# =============================================================================

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     AYUDA RAPIDA - PENSION BENEDITO                          " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "COMANDOS BASICOS:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  Verificar configuracion:"
Write-Host "    .\Scripts\verify.ps1"
Write-Host ""
Write-Host "  Configurar GitHub (una sola vez):"
Write-Host "    .\Scripts\setup.ps1 -UserName 'Tu Nombre' -UserEmail 'tu@email.com'"
Write-Host ""
Write-Host "  Menu interactivo:"
Write-Host "    .\Scripts\quick-start.ps1"
Write-Host ""

Write-Host "FLUJO DIARIO:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  1. Al empezar el dia:"
Write-Host "     git pull"
Write-Host ""
Write-Host "  2. Para guardar cambios:"
Write-Host "     .\Scripts\commit.ps1 -Type 'feat' -Message 'Descripcion'"
Write-Host ""
Write-Host "  3. Para subir a GitHub:"
Write-Host "     .\Scripts\push.ps1"
Write-Host ""
Write-Host "  4. Para crear backup:"
Write-Host "     .\Scripts\backup.ps1 -Version 'v1.0.0' -Message 'Version'"
Write-Host ""

Write-Host "TIPOS DE COMMIT:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  feat:     Nueva caracteristica"
Write-Host "  fix:      Correccion de bug"
Write-Host "  docs:     Documentacion"
Write-Host "  style:    Formato, espacios"
Write-Host "  refactor: Reestructurar codigo"
Write-Host "  test:     Tests"
Write-Host "  chore:    Mantenimiento"
Write-Host ""

Write-Host "COMANDOS GIT UTILES:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  git status                    Ver estado"
Write-Host "  git log --oneline             Ver historial"
Write-Host "  git diff                      Ver cambios"
Write-Host "  git branch                    Ver ramas"
Write-Host "  git stash                     Guardar cambios temporalmente"
Write-Host "  git stash pop                 Recuperar cambios guardados"
Write-Host ""

Write-Host "SOLUCION DE PROBLEMAS:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  No puedo hacer push:"
Write-Host "    git pull --rebase"
Write-Host "    git push"
Write-Host ""
Write-Host "  Hice un commit incorrecto (sin subir):"
Write-Host "    git reset --soft HEAD~1"
Write-Host ""
Write-Host "  Hice un commit incorrecto (ya subido):"
Write-Host "    git revert HEAD"
Write-Host "    git push"
Write-Host ""

Write-Host "DOCUMENTACION:" -ForegroundColor Yellow
Write-Host "────────────────────────────────────────────────────────────────"
Write-Host ""
Write-Host "  GITHUB_QUICKSTART.md   Guia rapida (3 pasos)"
Write-Host "  GITHUB_SETUP.md        Guia completa"
Write-Host "  GITHUB_READY.md        Estado actual"
Write-Host "  README.md              Documentacion del proyecto"
Write-Host ""

Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""