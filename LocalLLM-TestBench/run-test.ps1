<#
.SYNOPSIS
    Ejecuta un test contra el modelo local de llama.cpp

.DESCRIPTION
    Envía un prompt al modelo local y guarda el resultado con timestamp.
    Facilita la ejecución consistente de tests y el tracking de resultados.

.PARAMETER Category
    Categoría del test (01-boilerplate, 02-refactor, 03-documentation, 04-complex)

.PARAMETER TestName
    Nombre descriptivo del test (sin espacios, usar guiones)

.PARAMETER PromptFile
    Ruta al archivo de prompt (opcional, si no se especifica usa el prompt por defecto)

.PARAMETER MaxTokens
    Máximo de tokens para la respuesta (default: 1500)

.PARAMETER Temperature
    Temperatura del modelo (default: 0.3)

.EXAMPLE
    .\run-test.ps1 -Category "03-documentation" -TestName "inventory-docs"

.EXAMPLE
    .\run-test.ps1 -Category "01-boilerplate" -TestName "weapon-component" -MaxTokens 2000
#>

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("01-boilerplate", "02-refactor", "03-documentation", "04-complex")]
    [string]$Category,
    
    [Parameter(Mandatory=$true)]
    [string]$TestName,
    
    [string]$PromptFile,
    
    [int]$MaxTokens = 1500,
    
    [float]$Temperature = 0.3
)

$ErrorActionPreference = "Stop"

# Config
$endpoint = "http://localhost:8080/v1/chat/completions"
$apiKey = "local"
$basePath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\LocalLLM-TestBench"

# Read prompt
if ($PromptFile) {
    if (-not (Test-Path $PromptFile)) {
        Write-Error "Prompt file not found: $PromptFile"
        exit 1
    }
    $prompt = Get-Content $PromptFile -Raw
} else {
    # Try to find default prompt in category
    $defaultPrompt = Join-Path $basePath "tests\$Category\prompt.md"
    if (Test-Path $defaultPrompt) {
        $prompt = Get-Content $defaultPrompt -Raw
    } else {
        Write-Error "No prompt file specified and no default prompt found at: $defaultPrompt"
        exit 1
    }
}

# Prepare request
$headers = @{
    "Authorization" = "Bearer $apiKey"
    "Content-Type" = "application/json"
}

$body = @{
    model = "qwen"
    messages = @(
        @{
            role = "system"
            content = "You are an expert Unreal Engine 4.27.2 C++ developer. Generate clean, correct code following Epic's conventions."
        }
        @{
            role = "user"
            content = $prompt
        }
    )
    max_tokens = $MaxTokens
    temperature = $Temperature
} | ConvertTo-Json -Depth 3

# Execute request
Write-Host "🤖 Sending test '$TestName' to local model..." -ForegroundColor Cyan
Write-Host "   Category: $Category"
Write-Host "   Max Tokens: $MaxTokens"
Write-Host ""

try {
    $response = Invoke-RestMethod -Uri $endpoint -Method Post -Headers $headers -Body $body -TimeoutSec 120
    $generatedCode = $response.choices[0].message.content
    
    # Save result
    $timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
    $resultDir = Join-Path $basePath "tests\$Category"
    $resultPath = Join-Path $resultDir "result-$TestName-$timestamp.md"
    
    $content = @"
# Test Result — $TestName

**Timestamp**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Category**: $Category
**Model**: Qwen 3.8 27B

## Generated Code

``````cpp
$generatedCode
``````

## Metadata

- **Tokens used**: $($response.usage.total_tokens)
- **Prompt tokens**: $($response.usage.prompt_tokens)
- **Completion tokens**: $($response.usage.completion_tokens)
- **Temperature**: $Temperature
"@
    
    $content | Out-File -FilePath $resultPath -Encoding UTF8
    
    Write-Host "✅ Test completed!" -ForegroundColor Green
    Write-Host "   Result saved to: $resultPath"
    Write-Host ""
    Write-Host "📊 Token Usage:" -ForegroundColor Yellow
    Write-Host "   Prompt: $($response.usage.prompt_tokens)"
    Write-Host "   Completion: $($response.usage.completion_tokens)"
    Write-Host "   Total: $($response.usage.total_tokens)"
    Write-Host ""
    Write-Host "📝 Preview (first 500 chars):" -ForegroundColor Magenta
    Write-Host $generatedCode.Substring(0, [Math]::Min(500, $generatedCode.Length))
    
} catch {
    Write-Host "❌ Test failed: $_" -ForegroundColor Red
    exit 1
}