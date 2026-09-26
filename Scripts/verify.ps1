# =============================================================================
# Script de Verificacion del Proyecto
# =============================================================================
# Verifica que todo esta configurado correctamente
# Uso: .\Scripts\verify.ps1
# =============================================================================

$ErrorActionPreference = "Continue"

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     VERIFICACION DEL PROYECTO                                 " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

$checks = @()
$passed = 0
$failed = 0

# Check 1: Git installed
Write-Host "[1/8] Verificando Git..." -ForegroundColor Yellow
$gitVersion = git --version 2>&1
if ($gitVersion -match "git version") {
    Write-Host "   [OK] Git instalado: $gitVersion" -ForegroundColor Green
    $checks += @{Name="Git"; Status="OK"}
    $passed++
} else {
    Write-Host "   [ERROR] Git no encontrado" -ForegroundColor Red
    $checks += @{Name="Git"; Status="FAIL"}
    $failed++
}
Write-Host ""

# Check 2: GitHub CLI installed
Write-Host "[2/8] Verificando GitHub CLI..." -ForegroundColor Yellow
$ghVersion = gh --version 2>&1
if ($ghVersion -match "gh version") {
    Write-Host "   [OK] GitHub CLI instalado: $($ghVersion[0])" -ForegroundColor Green
    $checks += @{Name="GitHub CLI"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] GitHub CLI no encontrado (opcional)" -ForegroundColor Yellow
    $checks += @{Name="GitHub CLI"; Status="WARN"}
}
Write-Host ""

# Check 3: Git identity configured
Write-Host "[3/8] Verificando identidad de Git..." -ForegroundColor Yellow
$gitName = git config --global user.name 2>&1
$gitEmail = git config --global user.email 2>&1
if ($gitName -and $gitEmail) {
    Write-Host "   [OK] Usuario: $gitName" -ForegroundColor Green
    Write-Host "   [OK] Email: $gitEmail" -ForegroundColor Green
    $checks += @{Name="Git Identity"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] Identidad no configurada" -ForegroundColor Yellow
    Write-Host "   Ejecuta: git config --global user.name 'Tu Nombre'" -ForegroundColor Gray
    $checks += @{Name="Git Identity"; Status="WARN"}
}
Write-Host ""

# Check 4: Repository initialized
Write-Host "[4/8] Verificando repositorio Git..." -ForegroundColor Yellow
if (Test-Path ".git") {
    Write-Host "   [OK] Repositorio Git inicializado" -ForegroundColor Green
    $checks += @{Name="Git Repo"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] Repositorio no inicializado" -ForegroundColor Yellow
    $checks += @{Name="Git Repo"; Status="WARN"}
}
Write-Host ""

# Check 5: Git LFS installed
Write-Host "[5/8] Verificando Git LFS..." -ForegroundColor Yellow
$lfsVersion = git lfs version 2>&1
if ($lfsVersion -match "git-lfs") {
    Write-Host "   [OK] Git LFS instalado" -ForegroundColor Green
    $checks += @{Name="Git LFS"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] Git LFS no encontrado" -ForegroundColor Yellow
    $checks += @{Name="Git LFS"; Status="WARN"}
}
Write-Host ""

# Check 6: .gitignore exists
Write-Host "[6/8] Verificando .gitignore..." -ForegroundColor Yellow
if (Test-Path ".gitignore") {
    Write-Host "   [OK] .gitignore existe" -ForegroundColor Green
    $checks += @{Name=".gitignore"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] .gitignore no encontrado" -ForegroundColor Yellow
    $checks += @{Name=".gitignore"; Status="WARN"}
}
Write-Host ""

# Check 7: .gitattributes exists
Write-Host "[7/8] Verificando .gitattributes..." -ForegroundColor Yellow
if (Test-Path ".gitattributes") {
    Write-Host "   [OK] .gitattributes existe" -ForegroundColor Green
    $checks += @{Name=".gitattributes"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] .gitattributes no encontrado" -ForegroundColor Yellow
    $checks += @{Name=".gitattributes"; Status="WARN"}
}
Write-Host ""

# Check 8: Scripts exist
Write-Host "[8/8] Verificando scripts..." -ForegroundColor Yellow
$scripts = @("setup.ps1", "commit.ps1", "push.ps1", "backup.ps1", "quick-start.ps1")
$scriptsOk = 0
foreach ($script in $scripts) {
    if (Test-Path "Scripts\$script") {
        $scriptsOk++
    }
}
if ($scriptsOk -eq $scripts.Count) {
    Write-Host "   [OK] Todos los scripts encontrados ($scriptsOk/$($scripts.Count))" -ForegroundColor Green
    $checks += @{Name="Scripts"; Status="OK"}
    $passed++
} else {
    Write-Host "   [!] Faltan scripts ($scriptsOk/$($scripts.Count))" -ForegroundColor Yellow
    $checks += @{Name="Scripts"; Status="WARN"}
}
Write-Host ""

# Summary
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     RESUMEN                                                  " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Verificaciones completadas: $($checks.Count)" -ForegroundColor White
Write-Host "  - Pasadas: $passed" -ForegroundColor Green
Write-Host "  - Advertencias: $($checks.Count - $passed - $failed)" -ForegroundColor Yellow
Write-Host "  - Fallidas: $failed" -ForegroundColor Red
Write-Host ""

if ($failed -eq 0) {
    Write-Host "[OK] Todo esta listo para usar GitHub!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Siguiente paso:" -ForegroundColor Yellow
    Write-Host "   .\Scripts\setup.ps1 -UserName 'Tu Nombre' -UserEmail 'tu@email.com'" -ForegroundColor White
} else {
    Write-Host "[!] Hay problemas que resolver antes de continuar" -ForegroundColor Red
}

Write-Host ""