# Script de Investigación Mejorada para Pension Benedito
# Usa el modelo local con skills optimizados

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("historian", "visual", "atmosphere", "verify")]
    [string]$Skill,
    
    [Parameter(Mandatory=$true)]
    [string]$Prompt,
    
    [int]$MaxTokens = 2000,
    [float]$Temperature = 0.4
)

$ErrorActionPreference = "Stop"

# Configuration
$endpoint = "http://localhost:8080/v1/chat/completions"
$apiKey = "local"
$basePath = "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects\PensionBenedito\Research"

# System prompts for each skill
$systemPrompts = @{
    "historian" = @"
Eres un historiador profesional especializado en la Espana de 1930-1936.

REGLAS ESTRICTAS:
1. SOLO proporciona informacion VERIFICABLE y ESPECIFICA
2. Incluye SIEMPRE: nombres propios, fechas exactas, lugares precisos
3. NUNCA repitas la misma informacion con diferentes palabras
4. Si no tienes datos especificos, di "No tengo datos precisos sobre esto"
5. USA FORMATO ESTRUCTURADO con encabezados claros
6. MAXIMO 2000 tokens

FORMATO DE RESPUESTA:
- Encabezados numerados
- Listas con vinetas para detalles
- Datos entre parentesis cuando sea relevante
- Sin parrafos largos (maximo 3-4 lineas)

EVITA:
- Generalizaciones vagas
- Repeticiones
- Informacion no verificable
"@

    "visual" = @"
Eres un investigador visual especializado en fotografia historica.

TAREA: Proporciona REFERENCIAS ESPECIFICAS para encontrar imagenes.

FORMATO:
1. ARCHIVO/MUSEO: [Nombre exacto]
2. COLECCION: [Nombre de la coleccion]
3. BUSQUEDA: [Terminos exactos de busqueda]
4. EPOCA: [Rango de fechas]
5. TIPO: [Fotografia/Documento/Objeto]
6. URL si la conoces

EVITA: Generalizaciones. Se ESPECIFICO con nombres reales.
"@

    "atmosphere" = @"
Eres un disenador de entornos para videojuegos ambientados en los anos 30.

TAREA: Describe ELEMENTOS VISUALES concretos para recrear la epoca.

FORMATO POR ELEMENTO:
1. NOMBRE: [Que es]
2. APARIENCIA: [Como se ve - colores, texturas, materiales]
3. ESTADO: [Nuevo/usado/viejo/deteriorado]
4. UBICACION: [Donde se encontraria]
5. DETALLES: [Marcas, roturas, modificaciones]

MAXIMO 5 elementos por respuesta. Se DETALLADO y ESPECIFICO.
"@

    "verify" = @"
Eres un verificador de precision historica.

TAREA: Evalua la precision de la informacion proporcionada.

FORMATO:
1. AFIRMACION: [Cita la afirmacion]
2. VERIFICACION: [VERIFICADO/PROBABLE/NO VERIFICADO/APCRIFO]
3. EVIDENCIA: [Fuentes que lo confirman o niegan]
4. CORRECCION: [Si es necesario]

Se OBJETIVO y CRITICO. Marca todo lo que no puedas verificar.
"@
}

# Validate skill
if (-not $systemPrompts.ContainsKey($Skill)) {
    Write-Error "Invalid skill: $Skill. Valid skills: $($systemPrompts.Keys -join ', ')"
    exit 1
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
            content = $systemPrompts[$Skill]
        }
        @{
            role = "user"
            content = $Prompt
        }
    )
    max_tokens = $MaxTokens
    temperature = $Temperature
} | ConvertTo-Json -Depth 3

# Execute request
Write-Host "Enviando investigacion con skill '$Skill'..." -ForegroundColor Cyan
Write-Host "Prompt: $($Prompt.Substring(0, [Math]::Min(100, $Prompt.Length)))..." -ForegroundColor Gray
Write-Host ""

try {
    $response = Invoke-RestMethod -Uri $endpoint -Method Post -Headers $headers -Body $body -TimeoutSec 180
    $result = $response.choices[0].message.content
    
    # Save result
    $timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
    $resultDir = Join-Path $basePath "Results"
    if (-not (Test-Path $resultDir)) {
        New-Item -ItemType Directory -Path $resultDir | Out-Null
    }
    
    $resultPath = Join-Path $resultDir "$Skill-$timestamp.md"
    
    $content = @"
# Investigacion: $Skill

**Fecha**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
**Skill**: $Skill
**Prompt**: $Prompt

---

$result

---

## Metadata

- Tokens totales: $($response.usage.total_tokens)
- Tokens prompt: $($response.usage.prompt_tokens)
- Tokens respuesta: $($response.usage.completion_tokens)
"@
    
    $content | Out-File -FilePath $resultPath -Encoding UTF8
    
    Write-Host "Investigacion completada!" -ForegroundColor Green
    Write-Host "   Archivo: $resultPath" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Estadisticas:" -ForegroundColor Yellow
    Write-Host "   Tokens totales: $($response.usage.total_tokens)"
    Write-Host "   Tokens respuesta: $($response.usage.completion_tokens)"
    Write-Host ""
    Write-Host "Preview:" -ForegroundColor Magenta
    Write-Host $result.Substring(0, [Math]::Min(500, $result.Length))
    
} catch {
    Write-Host "Error: $_" -ForegroundColor Red
    exit 1
}