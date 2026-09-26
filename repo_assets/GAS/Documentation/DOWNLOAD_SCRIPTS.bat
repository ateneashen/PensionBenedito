@echo off
REM ============================================================
REM GAS Repository Download Scripts
REM ============================================================
REM 
REM This script downloads the essential GAS educational repositories
REM for learning Unreal Engine 4.27 Gameplay Ability System.
REM 
REM Prerequisites:
REM - Git installed (https://git-scm.com/)
REM - Sufficient disk space (~500MB for all repos)
REM - Internet connection
REM 
REM ============================================================

echo ============================================================
echo GAS Educational Repository Downloader
echo ============================================================
echo.

set GAS_DIR=C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\repo_assets\GAS

REM Create directory structure
echo Creating directory structure...
mkdir "%GAS_DIR%\Documentation" 2>nul
mkdir "%GAS_DIR%\Samples" 2>nul
mkdir "%GAS_DIR%\TutorialProjects" 2>nul
mkdir "%GAS_DIR%\Reference" 2>nul
mkdir "%GAS_DIR%\Templates" 2>nul

echo.
echo ============================================================
echo TIER 1: ESSENTIAL REPOSITORIES
echo ============================================================
echo.

REM ------------------------------------------------------------
REM 1. GASDocumentation (Tranek)
REM ------------------------------------------------------------
echo [1/5] Downloading GASDocumentation...
echo      This is THE essential GAS reference document
echo      URL: https://github.com/tranek/GASDocumentation
echo.

cd /d "%GAS_DIR%\Documentation"
if exist "GASDocumentation" (
    echo      [SKIP] GASDocumentation already exists
    echo      Updating instead...
    cd GASDocumentation
    git pull
    cd ..
) else (
    git clone https://github.com/tranek/GASDocumentation.git
    if errorlevel 1 (
        echo      [ERROR] Failed to clone GASDocumentation
        echo      Try manually: https://github.com/tranek/GASDocumentation
    ) else (
        echo      [OK] GASDocumentation downloaded successfully
    )
)

echo.

REM ------------------------------------------------------------
REM 2. ActionRPG (Epic Games)
REM ------------------------------------------------------------
echo [2/5] Downloading ActionRPG (Epic's Official Sample)...
echo      This is the canonical GAS example from Epic Games
echo      URL: https://github.com/ue4plugins/ActionRPG
echo.

cd /d "%GAS_DIR%\Samples"
if exist "ActionRPG" (
    echo      [SKIP] ActionRPG already exists
    echo      Updating instead...
    cd ActionRPG
    git pull
    cd ..
) else (
    git clone https://github.com/ue4plugins/ActionRPG.git
    if errorlevel 1 (
        echo      [ERROR] Failed to clone ActionRPG
        echo      Try manually: https://github.com/ue4plugins/ActionRPG
    ) else (
        echo      [OK] ActionRPG downloaded successfully
    )
)

echo.

echo ============================================================
echo TIER 2: HIGHLY RECOMMENDED
echo ============================================================
echo.

REM ------------------------------------------------------------
REM 3. Search for DruidMech GAS Projects
REM ------------------------------------------------------------
echo [3/5] Checking DruidMech repositories...
echo      Visit: https://github.com/DruidMech
echo.
echo      Manual Action Required:
echo      1. Go to https://github.com/DruidMech
echo      2. Look for GAS-related repositories
echo      3. Clone any GAS tutorial projects
echo.
echo      Common DruidMech repos to look for:
echo      - GASDocumentation-ExampleProject
echo      - UnrealGAS
echo      - GAS-Tutorial-Series
echo.

REM Placeholder for DruidMech clone
REM Uncomment and modify when you find the specific repo:
REM cd /d "%GAS_DIR%\TutorialProjects"
REM git clone https://github.com/DruidMech/GAS-TUTORIAL-REPO-NAME.git

echo.

REM ------------------------------------------------------------
REM 4. UE4 GAS Template
REM ------------------------------------------------------------
echo [4/5] Downloading UE4 GAS Template Project...
echo      Search GitHub for: "UE4 Gameplay Ability System template"
echo.
echo      Recommended searches:
echo      - "UE4.27 GAS template"
echo      - "Gameplay Ability System starter"
echo      - "GAS third person template"
echo.
echo      Manual Action Required:
echo      1. Search GitHub for UE4 GAS templates
echo      2. Find one compatible with UE4.27
echo      3. Clone to: %GAS_DIR%\Templates\
echo.

REM Example (replace with actual repo when found):
REM cd /d "%GAS_DIR%\Templates"
REM git clone https://github.com/AUTHOR/UE4-GAS-TEMPLATE.git

echo.

echo ============================================================
echo TIER 3: SUPPLEMENTARY (Optional)
echo ============================================================
echo.

REM ------------------------------------------------------------
REM 5. Lyra Reference (Patterns Only)
REM ------------------------------------------------------------
echo [5/5] Lyra Starter Game Reference...
echo      Note: Lyra is UE5, but patterns are educational
echo      Requires Epic Games access: https://github.com/EpicGames/UnrealEngine
echo.
echo      Alternative: Search for Lyra GAS documentation/explanations
echo      - "Lyra GAS architecture"
echo      - "Lyra Gameplay Ability System patterns"
echo.
echo      Extracted patterns can be saved to:
echo      %GAS_DIR%\Reference\Lyra-Patterns\
echo.

echo ============================================================
echo DOWNLOAD COMPLETE
echo ============================================================
echo.

echo Next Steps:
echo.
echo 1. READ GASDocumentation FIRST
echo    Location: %GAS_DIR%\Documentation\GASDocumentation\
echo    Start with: README.md
echo    Time: 2-3 days for thorough reading
echo.
echo 2. STUDY ActionRPG Sample
echo    Location: %GAS_DIR%\Samples\ActionRPG\
echo    Key files to examine:
echo    - Source/ActionRPG/Abilities/
echo    - Source/ActionRPG/Character/
echo    - Source/ActionRPG/Attributes/
echo    Time: 1 week
echo.
echo 3. BUILD YOUR FIRST ABILITY
echo    Use ActionRPG as reference
echo    Create simple projectile ability
echo    Test in a blank UE4.27 project
echo.
echo 4. CHECK DruidMech for tutorials
echo    Visit: https://github.com/DruidMech
echo    Look for video tutorial companions
echo.
echo ============================================================
echo.

pause

REM ============================================================
REM MANUAL DOWNLOAD INSTRUCTIONS (if script fails)
REM ============================================================ 
REM 
REM If the batch script fails, use these manual commands:
REM
REM 1. GASDocumentation:
REM    cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\repo_assets\GAS\Documentation"
REM    git clone https://github.com/tranek/GASDocumentation.git
REM
REM 2. ActionRPG:
REM    cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\repo_assets\GAS\Samples"
REM    git clone https://github.com/ue4plugins/ActionRPG.git
REM
REM 3. DruidMech Projects:
REM    Visit https://github.com/DruidMech
REM    Find GAS repos and clone manually
REM
REM ============================================================