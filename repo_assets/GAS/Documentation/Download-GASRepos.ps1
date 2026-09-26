# ============================================================
# GAS Repository Download Script (PowerShell)
# ============================================================
#
# Downloads essential GAS educational repositories for UE4.27
#
# Usage:
#   .\Download-GASRepos.ps1
#   .\Download-GASRepos.ps1 -SkipExisting
#   .\Download-GASRepos.ps1 -Tier 1
#
# ============================================================

param(
    [switch]$SkipExisting,
    [int]$Tier = 3  # 1=Essential, 2=Recommended, 3=All
)

# Configuration
$GAS_DIR = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\repo_assets\GAS"
$REPOS = @(
    # Tier 1: Essential
    @{
        Name = "GASDocumentation"
        URL = "https://github.com/tranek/GASDocumentation.git"
        Path = "$GAS_DIR\Documentation\GASDocumentation"
        Priority = 1
        Description = "THE essential GAS reference document (Tranek)"
    },
    @{
        Name = "ActionRPG"
        URL = "https://github.com/ue4plugins/ActionRPG.git"
        Path = "$GAS_DIR\Samples\ActionRPG"
        Priority = 1
        Description = "Epic's official Action RPG GAS sample"
    },
    
    # Tier 2: Highly Recommended
    @{
        Name = "DruidMech-Profile"
        URL = "https://github.com/DruidMech"
        Path = "$GAS_DIR\TutorialProjects\DruidMech"
        Priority = 2
        Description = "Visit DruidMech's GitHub for GAS tutorials"
        Manual = $true
    },
    
    # Tier 3: Supplementary
    @{
        Name = "Lyra-Patterns-Reference"
        URL = "N/A (UE5 Reference)"
        Path = "$GAS_DIR\Reference\Lyra-Patterns"
        Priority = 3
        Description = "Lyra GAS patterns for reference (UE5)"
        Manual = $true
    }
)

# Functions
function Write-Header {
    param([string]$Text)
    Write-Host ""
    Write-Host "=" * 60 -ForegroundColor Cyan
    Write-Host $Text -ForegroundColor Cyan
    Write-Host "=" * 60 -ForegroundColor Cyan
    Write-Host ""
}

function Write-Step {
    param([string]$Step, [string]$Message)
    Write-Host "[$Step] " -ForegroundColor Yellow -NoNewline
    Write-Host $Message
}

function Write-Success {
    param([string]$Message)
    Write-Host "  [OK] " -ForegroundColor Green -NoNewline
    Write-Host $Message
}

function Write-Skip {
    param([string]$Message)
    Write-Host "  [SKIP] " -ForegroundColor DarkGray -NoNewline
    Write-Host $Message
}

function Write-Warning {
    param([string]$Message)
    Write-Host "  [!] " -ForegroundColor Yellow -NoNewline
    Write-Host $Message
}

function Write-Error {
    param([string]$Message)
    Write-Host "  [ERROR] " -ForegroundColor Red -NoNewline
    Write-Host $Message
}

function Test-GitInstalled {
    try {
        $gitVersion = git --version 2>&1
        return $true
    }
    catch {
        return $false
    }
}

function Clone-Repository {
    param(
        [string]$Name,
        [string]$URL,
        [string]$Path,
        [switch]$SkipExisting
    )
    
    # Check if directory exists
    if (Test-Path $Path) {
        if ($SkipExisting) {
            Write-Skip "$Name already exists (use -SkipExisting:`$false to update)"
            return $true
        }
        else {
            Write-Warning "$Name exists, attempting to update..."
            try {
                Push-Location $Path
                git pull
                Pop-Location
                Write-Success "$Name updated successfully"
                return $true
            }
            catch {
                Write-Error "Failed to update $Name`: $_"
                Pop-Location
                return $false
            }
        }
    }
    
    # Clone repository
    Write-Host "  Cloning from: $URL" -ForegroundColor DarkGray
    try {
        # Ensure parent directory exists
        $parentDir = Split-Path $Path -Parent
        if (-not (Test-Path $parentDir)) {
            New-Item -ItemType Directory -Path $parentDir -Force | Out-Null
        }
        
        git clone $URL $Path 2>&1 | Out-Null
        Write-Success "$Name downloaded successfully"
        return $true
    }
    catch {
        Write-Error "Failed to clone $Name`: $_"
        Write-Host "  Manual command: git clone $URL `"$Path`"" -ForegroundColor DarkGray
        return $false
    }
}

# Main Script
Write-Header "GAS Educational Repository Downloader"

# Check prerequisites
Write-Step "PREREQ" "Checking prerequisites..."

if (-not (Test-GitInstalled)) {
    Write-Error "Git is not installed or not in PATH"
    Write-Host ""
    Write-Host "Please install Git from: https://git-scm.com/" -ForegroundColor Yellow
    Write-Host "After installation, restart PowerShell and try again." -ForegroundColor Yellow
    exit 1
}
else {
    Write-Success "Git is installed"
}

# Create directory structure
Write-Step "SETUP" "Creating directory structure..."

$directories = @(
    "$GAS_DIR\Documentation",
    "$GAS_DIR\Samples",
    "$GAS_DIR\TutorialProjects",
    "$GAS_DIR\Reference",
    "$GAS_DIR\Templates"
)

foreach ($dir in $directories) {
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
        Write-Host "  Created: $dir" -ForegroundColor DarkGray
    }
}

Write-Success "Directory structure ready"

# Download repositories by tier
$successCount = 0
$failCount = 0
$skipCount = 0

foreach ($repo in $REPOS) {
    # Skip repos above current tier
    if ($repo.Priority -gt $Tier) {
        continue
    }
    
    # Handle manual repos differently
    if ($repo.Manual) {
        Write-Warning "$($repo.Name) requires manual action"
        Write-Host "  $($repo.Description)" -ForegroundColor DarkGray
        Write-Host "  Visit: $($repo.URL)" -ForegroundColor DarkGray
        Write-Host ""
        continue
    }
    
    $result = Clone-Repository -Name $repo.Name -URL $repo.URL -Path $repo.Path -SkipExisting:$SkipExisting
    
    if ($result) {
        $successCount++
    }
    else {
        $failCount++
    }
}

# Summary
Write-Header "Download Summary"

Write-Host "Successfully downloaded: " -NoNewline
Write-Host "$successCount" -ForegroundColor Green

if ($failCount -gt 0) {
    Write-Host "Failed: " -NoNewline
    Write-Host "$failCount" -ForegroundColor Red
}

Write-Host ""

# Next Steps
Write-Header "Next Steps"

Write-Host "1. READ GASDocumentation FIRST" -ForegroundColor Cyan
Write-Host "   Location: $GAS_DIR\Documentation\GASDocumentation\" -ForegroundColor DarkGray
Write-Host "   Start with: README.md" -ForegroundColor DarkGray
Write-Host "   Time: 2-3 days for thorough reading" -ForegroundColor DarkGray
Write-Host ""

Write-Host "2. STUDY ActionRPG Sample" -ForegroundColor Cyan
Write-Host "   Location: $GAS_DIR\Samples\ActionRPG\" -ForegroundColor DarkGray
Write-Host "   Key files to examine:" -ForegroundColor DarkGray
Write-Host "   - Source/ActionRPG/Abilities/" -ForegroundColor DarkGray
Write-Host "   - Source/ActionRPG/Character/" -ForegroundColor DarkGray
Write-Host "   - Source/ActionRPG/Attributes/" -ForegroundColor DarkGray
Write-Host "   Time: 1 week" -ForegroundColor DarkGray
Write-Host ""

Write-Host "3. VISIT DruidMech for tutorials" -ForegroundColor Cyan
Write-Host "   URL: https://github.com/DruidMech" -ForegroundColor DarkGray
Write-Host "   Look for GAS tutorial repositories" -ForegroundColor DarkGray
Write-Host "   Clone to: $GAS_DIR\TutorialProjects\" -ForegroundColor DarkGray
Write-Host ""

Write-Host "4. CHECK REPOSITORIES.MD for full details" -ForegroundColor Cyan
Write-Host "   Location: $GAS_DIR\Documentation\REPOSITORIES.md" -ForegroundColor DarkGray
Write-Host ""

Write-Host "5. BUILD YOUR FIRST ABILITY" -ForegroundColor Cyan
Write-Host "   Use ActionRPG as reference" -ForegroundColor DarkGray
Write-Host "   Create simple projectile ability in UE4.27" -ForegroundColor DarkGray
Write-Host ""

Write-Header "Happy Learning!"

# Open folder in Explorer
$openFolder = Read-Host "Open GAS folder in Explorer? (Y/N)"
if ($openFolder -eq "Y" -or $openFolder -eq "y") {
    explorer.exe $GAS_DIR
}