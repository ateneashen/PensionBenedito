# Script de Investigación con Búsqueda Web
# Combina el modelo local con búsquedas reales en la web

param(
    [Parameter(Mandatory=$true)]
    [string]$Query,
    
    [string]$SessionName = "pension-benedito-research"
)

$ErrorActionPreference = "Stop"

# WebBridge endpoint
$webbridgeUrl = "http://127.0.0.1:10086/command"

# Function to send webbridge command
function Send-WebBridgeCommand {
    param(
        [string]$Action,
        [hashtable]$Args,
        [string]$Session
    )
    
    $body = @{
        action = $Action
        args = $Args
        session = $Session
    } | ConvertTo-Json -Depth 5
    
    $tempFile = "$env:TEMP\webbridge-req-$(Get-Random).json"
    $body | Out-File -FilePath $tempFile -Encoding UTF8
    
    try {
        $response = curl.exe -s -X POST $webbridgeUrl -H "Content-Type: application/json" --data-binary "@$tempFile"
        return $response | ConvertFrom-Json
    } finally {
        Remove-Item $tempFile -Force -ErrorAction SilentlyContinue
    }
}

# Function to search using Brave Search
function Search-Brave {
    param([string]$Query)
    
    Write-Host "Buscando en Brave: $Query" -ForegroundColor Cyan
    
    # Navigate to Brave Search
    $result = Send-WebBridgeCommand -Action "navigate" -Args @{
        url = "https://search.brave.com/search?q=$([System.Uri]::EscapeDataString($Query))"
        newTab = $true
        group_title = "Busqueda: $Query"
    } -Session $SessionName
    
    if (-not $result.ok) {
        Write-Host "Error navegando: $($result.error)" -ForegroundColor Red
        return $null
    }
    
    Start-Sleep -Seconds 2
    
    # Get snapshot of results
    $snapshot = Send-WebBridgeCommand -Action "snapshot" -Args @{} -Session $SessionName
    
    if ($snapshot.ok) {
        Write-Host "Resultados obtenidos" -ForegroundColor Green
        return $snapshot.tree
    }
    
    return $null
}

# Main execution
Write-Host "=== Investigacion con Busqueda Web ===" -ForegroundColor Yellow
Write-Host "Query: $Query" -ForegroundColor White
Write-Host ""

# Search in Brave
$results = Search-Brave -Query $Query

if ($results) {
    # Save results
    $timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
    $outputPath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\PensionBenedito\Research\Web-Results\search-$timestamp.md"
    
    $content = @"
# Busqueda Web: $Query

**Fecha**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Motor**: Brave Search

---

$resultados

---

## Siguientes Pasos

1. Revisar los resultados
2. Navegar a los enlaces relevantes
3. Extraer informacion util
4. Documentar hallazgos
"@
    
    $content | Out-File -FilePath $outputPath -Encoding UTF8
    Write-Host ""
    Write-Host "Resultados guardados en: $outputPath" -ForegroundColor Green
} else {
    Write-Host "No se obtuvieron resultados" -ForegroundColor Red
}