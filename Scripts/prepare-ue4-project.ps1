# =============================================================================
# Script para Preparar Proyecto UE4.27
# =============================================================================
# Crea el proyecto de Unreal Engine y vincula el codigo
# Uso: .\Scripts\prepare-ue4-project.ps1
# =============================================================================

param(
    [string]$ProjectName = "PensionBenedito",
    [string]$EnginePath = "C:\Program Files\Epic Games\UE_4.27"
)

$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host "     PREPARACION DE PROYECTO UE4.27                           " -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

$basePath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"
$projectPath = "$basePath\$ProjectName"

# Step 1: Verify UE4.27 installation
Write-Host "[PASO 1] Verificando UE4.27..." -ForegroundColor Yellow
if (-not (Test-Path $EnginePath)) {
    Write-Host "   [ERROR] UE4.27 no encontrado en: $EnginePath" -ForegroundColor Red
    Write-Host "   Por favor, instala UE4.27 desde Epic Games Launcher" -ForegroundColor Yellow
    exit 1
}

$buildTool = "$EnginePath\Engine\Build\BatchFiles\RunUAT.bat"
if (-not (Test-Path $buildTool)) {
    Write-Host "   [ERROR] RunUAT.bat no encontrado" -ForegroundColor Red
    exit 1
}

Write-Host "   [OK] UE4.27 encontrado" -ForegroundColor Green
Write-Host ""

# Step 2: Check if project already exists
Write-Host "[PASO 2] Verificando proyecto existente..." -ForegroundColor Yellow
$uprojectFile = "$projectPath\$ProjectName.uproject"
if (Test-Path $uprojectFile) {
    Write-Host "   [!] Proyecto ya existe: $uprojectFile" -ForegroundColor Yellow
    $overwrite = Read-Host "   ¿Deseas sobrescribir? (s/n)"
    if ($overwrite -ne "s" -and $overwrite -ne "S") {
        Write-Host "   [SKIP] Omitido por usuario" -ForegroundColor Yellow
        exit 0
    }
    Write-Host "   Eliminando proyecto existente..." -ForegroundColor Gray
    Remove-Item -Recurse -Force $projectPath
}

Write-Host "   [OK] Listo para crear proyecto" -ForegroundColor Green
Write-Host ""

# Step 3: Create .uproject file
Write-Host "[PASO 3] Creando archivo .uproject..." -ForegroundColor Yellow

$uprojectContent = @"
{
    "FileVersion": 3,
    "EngineAssociation": "4.27",
    "Category": "Game",
    "Description": "Horror mystery game set in 1930s Spain",
    "Modules": [
        {
            "Name": "$ProjectName",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        },
        {
            "Name": "NarrativeActionKit",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        }
    ],
    "Plugins": [
        {
            "Name": "GameplayAbilities",
            "Enabled": true
        },
        {
            "Name": "GameplayTags",
            "Enabled": true
        },
        {
            "Name": "GameplayTasks",
            "Enabled": true
        }
    ]
}
"@

# Create directory if needed
if (-not (Test-Path $projectPath)) {
    New-Item -ItemType Directory -Path $projectPath | Out-Null
}

$uprojectContent | Out-File -FilePath $uprojectFile -Encoding UTF8
Write-Host "   [OK] Archivo .uproject creado" -ForegroundColor Green
Write-Host ""

# Step 4: Create Source directory structure
Write-Host "[PASO 4] Creando estructura de Source..." -ForegroundColor Yellow

$sourceDirs = @(
    "$projectPath\Source\$ProjectName",
    "$projectPath\Source\$ProjectName\Public",
    "$projectPath\Source\$ProjectName\Private",
    "$projectPath\Source\NarrativeActionKit",
    "$projectPath\Source\NarrativeActionKit\Public",
    "$projectPath\Source\NarrativeActionKit\Private",
    "$projectPath\Content",
    "$projectPath\Content\Blueprints",
    "$projectPath\Content\Maps",
    "$projectPath\Content\UI",
    "$projectPath\Config"
)

foreach ($dir in $sourceDirs) {
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
}

Write-Host "   [OK] Estructura creada" -ForegroundColor Green
Write-Host ""

# Step 5: Create Build.cs for main module
Write-Host "[PASO 5] Creando Build.cs..." -ForegroundColor Yellow

$buildCsContent = @"
using UnrealBuildTool;

public class $ProjectName : ModuleRules
{
    public `$ProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "UMG",
            "NavigationSystem",
            "NarrativeActionKit"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
    }
}
"@

$buildCsContent | Out-File -FilePath "$projectPath\Source\$ProjectName\$ProjectName.Build.cs" -Encoding UTF8
Write-Host "   [OK] Build.cs creado" -ForegroundColor Green
Write-Host ""

# Step 6: Create module .h and .cpp
Write-Host "[PASO 6] Creando archivos de modulo..." -ForegroundColor Yellow

# Module .h
$moduleH = @"
#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Log$ProjectName, Log, All);
"@
$moduleH | Out-File -FilePath "$projectPath\Source\$ProjectName\$ProjectName.h" -Encoding UTF8

# Module .cpp
$moduleCpp = @"
#include "$ProjectName.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, $ProjectName, "$ProjectName");

DEFINE_LOG_CATEGORY(Log$ProjectName);
"@
$moduleCpp | Out-File -FilePath "$projectPath\Source\$ProjectName\$ProjectName.cpp" -Encoding UTF8

Write-Host "   [OK] Archivos de modulo creados" -ForegroundColor Green
Write-Host ""

# Step 7: Create Config files
Write-Host "[PASO 7] Creando archivos de configuracion..." -ForegroundColor Yellow

# DefaultEngine.ini
$engineConfig = @"
[/Script/EngineSettings.GameMapsSettings]
EditorStartupMap=/Game/Maps/Default
GameDefaultMap=/Game/Maps/Default
GlobalDefaultGameMode=/Game/Blueprints/BP_GameMode.BP_GameMode_C

[/Script/Engine.Engine]
+ActiveGameNameRedirects=(OldGameName="TP_Blank",NewGameName="/Script/$ProjectName")
+ActiveGameNameRedirects=(OldGameName="/Script/TP_Blank",NewGameName="/Script/$ProjectName")
"@
$engineConfig | Out-File -FilePath "$projectPath\Config\DefaultEngine.ini" -Encoding UTF8

# DefaultGame.ini
$gameConfig = ""
$gameConfig | Out-File -FilePath "$projectPath\Config\DefaultGame.ini" -Encoding UTF8

# DefaultInput.ini
$inputConfig = @"
[/Script/Engine.InputSettings]
-AxisConfig=(AxisKeyName="Gamepad_LeftX",DeadZone=0.25,Exponent=1.0,bInvert=False)
+AxisConfig=(AxisKeyName="Gamepad_LeftX",DeadZone=0.25,Exponent=1.0,bInvert=False)

+ActionMappings=(ActionName="Interact",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=E)
+ActionMappings=(ActionName="Inventory",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=I)
+ActionMappings=(ActionName="Pause",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Escape)

+AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)
+AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=S)
+AxisMappings=(AxisName="MoveRight",Scale=1.000000,Key=D)
+AxisMappings=(AxisName="MoveRight",Scale=-1.000000,Key=A)
+AxisMappings=(AxisName="Turn",Scale=1.000000,Key=MouseX)
+AxisMappings=(AxisName="LookUp",Scale=-1.000000,Key=MouseY)
"@
$inputConfig | Out-File -FilePath "$projectPath\Config\DefaultInput.ini" -Encoding UTF8

Write-Host "   [OK] Archivos de configuracion creados" -ForegroundColor Green
Write-Host ""

# Step 8: Copy source code
Write-Host "[PASO 8] Copiando codigo fuente..." -ForegroundColor Yellow

# Copy NarrativeActionKit
$nakSource = "$basePath\NarrativeActionKit\Source\NarrativeActionKit"
$nakTarget = "$projectPath\Source\NarrativeActionKit"

if (Test-Path $nakSource) {
    Copy-Item -Path "$nakSource\*" -Destination $nakTarget -Recurse -Force
    Write-Host "   [OK] NarrativeActionKit copiado" -ForegroundColor Green
} else {
    Write-Host "   [!] NarrativeActionKit no encontrado" -ForegroundColor Yellow
}

# Copy PensionBenedito source
$pbSource = "$basePath\PensionBenedito\Source\PensionBenedito"
$pbTarget = "$projectPath\Source\$ProjectName"

if (Test-Path $pbSource) {
    Copy-Item -Path "$pbSource\Public\*" -Destination "$pbTarget\Public" -Recurse -Force
    Copy-Item -Path "$pbSource\Private\*" -Destination "$pbTarget\Private" -Recurse -Force
    Write-Host "   [OK] PensionBenedito copiado" -ForegroundColor Green
} else {
    Write-Host "   [!] PensionBenedito source no encontrado" -ForegroundColor Yellow
}

Write-Host ""

# Step 9: Generate project files
Write-Host "[PASO 9] Generando archivos de proyecto..." -ForegroundColor Yellow
Write-Host "   Esto puede tomar unos minutos..." -ForegroundColor Gray

$generateTool = "$EnginePath\Engine\Build\BatchFiles\GenerateProjectFiles.bat"
if (Test-Path $generateTool) {
    Set-Location $projectPath
    & $generateTool "$uprojectFile" 2>&1 | Out-Null
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "   [OK] Archivos de proyecto generados" -ForegroundColor Green
    } else {
        Write-Host "   [!] Error al generar archivos (puedes abrir el .uproject directamente)" -ForegroundColor Yellow
    }
} else {
    Write-Host "   [!] GenerateProjectFiles.bat no encontrado" -ForegroundColor Yellow
    Write-Host "   Abre el .uproject directamente en UE4.27" -ForegroundColor Gray
}

Write-Host ""

# Final summary
Write-Host "================================================================" -ForegroundColor Green
Write-Host "     PROYECTO CREADO EXITOSAMENTE                             " -ForegroundColor Green
Write-Host "================================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Ubicacion del proyecto:" -ForegroundColor Yellow
Write-Host "  $projectPath" -ForegroundColor Cyan
Write-Host ""
Write-Host "Archivo principal:" -ForegroundColor Yellow
Write-Host "  $uprojectFile" -ForegroundColor Cyan
Write-Host ""
Write-Host "Proximos pasos:" -ForegroundColor Yellow
Write-Host "  1. Abre el archivo .uproject con UE4.27" -ForegroundColor White
Write-Host "  2. Espera a que compile el codigo" -ForegroundColor White
Write-Host "  3. Crea los Blueprints necesarios" -ForegroundColor White
Write-Host "  4. ¡Empieza a desarrollar!" -ForegroundColor White
Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""