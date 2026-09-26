# Guia de Investigacion - Pension Benedito

## Estructura de la Investigacion

```
Research/
├── Historical-Context/         # Contexto historico (generado por modelo local)
├── Terror-Elements/            # Elementos de terror (generado por modelo local)
├── Visual-References/          # Referencias visuales (a llenar con webbridge)
├── Inspirations/               # Historias y referencias
├── Results/                    # Resultados de investigacion mejorada
├── Web-Results/                # Resultados de busquedas web
├── prompts-enhanced.md         # Skills y prompts optimizados
├── invoke-research.ps1         # Script para usar skills
└── web-search-research.ps1     # Script para busquedas web
```

## Estrategia de Investigacion

### FASE 1: Modelo Local (Completada ✅)
- Contexto historico basico
- Elementos de terror generales
- Identificacion de areas a investigar

### FASE 2: Modelo Local + Skills (En progreso)
- Usar skills optimizados para respuestas precisas
- Evitar bucles repetitivos
- Estructurar informacion util

### FASE 3: Busqueda Web (Pendiente)
- Usar webbridge para buscar en archivos historicos
- Encontrar fotografias de la epoca
- Verificar informacion del modelo local

### FASE 4: Documentacion Final (Pendiente)
- Consolidar toda la informacion
- Crear guia de referencia visual
- Documentar fuentes verificadas

---

## Skills Disponibles

### 1. Historian (`historian`)
**Uso**: Informacion historica precisa
**Ejemplo**:
```powershell
.\invoke-research.ps1 -Skill "historian" -Prompt "Top 5 trabajos comunes en Madrid 1930-1936"
```

### 2. Visual (`visual`)
**Uso**: Referencias para encontrar imagenes
**Ejemplo**:
```powershell
.\invoke-research.ps1 -Skill "visual" -Prompt "Donde encontrar fotos de interiores de pensiones en Madrid 1930s"
```

### 3. Atmosphere (`atmosphere`)
**Uso**: Descripcion de elementos visuales
**Ejemplo**:
```powershell
.\invoke-research.ps1 -Skill "atmosphere" -Prompt "Describe 5 objetos tipicos de una pension madrilena 1930"
```

### 4. Verify (`verify`)
**Uso**: Verificar precision de informacion
**Ejemplo**:
```powershell
.\invoke-research.ps1 -Skill "verify" -Prompt "Verifica: 'La pension X existia en la calle Y de Madrid en 1932'"
```

---

## Busquedas Web Recomendadas

### Archivos Historicos
1. **Archivo Regional de la Comunidad de Madrid**
   - URL: https://www.archivomadrid.es/
   - Buscar: "pensiones 1930", "vida cotidiana", "Madrid republicano"

2. **Hemeroteca Digital BNE**
   - URL: https://hemerotecadigital.bne.es/
   - Buscar: "pension", "hospedaje", "habitaciones amuebladas"

3. **Museo de Historia de Madrid**
   - URL: https://musartbcn.cat/
   - Buscar: "interiores 1930", "vivienda madrilena"

### Fotografias
1. **Archivo Fotografico de Madrid**
   - Buscar: "pension", "hotel", "hospedaje"
   - Epoca: 1930-1936

2. **Colecciones de Arte**
   - Buscar: "pintura costumbrista 1930"
   - Artistas: Solana, Benedito (si existe relacion)

### Documentos
1. **Padrones Municipales**
   - Registros de residentes en pensiones
   - Direcciones y nombres

2. **Guias de Viaje**
   - "Guia de Madrid" de la epoca
   - Listados de pensiones y hoteles

---

## Ejemplo de Investigacion Completa

### Objetivo: Encontrar informacion sobre la "Pension Benedito"

**Paso 1: Modelo Local (Historian)**
```powershell
.\invoke-research.ps1 -Skill "historian" -Prompt @'
Busca informacion sobre:
1. ¿Existia una pension llamada "Benedito" en Madrid o Barcelona 1930-1936?
2. Si no, ¿que pensiones famosas habia en ambos ciudades?
3. ¿Que apellido era comun para dueños de pensiones?
'@
```

**Paso 2: Busqueda Web**
```powershell
.\web-search-research.ps1 -Query "pension Benedito Madrid 1930"
```

**Paso 3: Verificacion**
```powershell
.\invoke-research.ps1 -Skill "verify" -Prompt "Verifica la existencia de pensiones con apellido Benedito en Madrid 1930-1936"
```

**Paso 4: Referencias Visuales**
```powershell
.\invoke-research.ps1 -Skill "visual" -Prompt "Donde encontrar fotos de pensiones madrilenas de los anos 30"
```

---

## Consejos para el Modelo Local

### Prompts Efectivos
1. **Se especifico**: "Top 5" en vez de "algunos"
2. **Pide formato**: "Lista numerada", "Tabla"
3. **Limita tokens**: Maximo 2000
4. **Evita abiertos**: "Describe X" es malo; "Lista 3 caracteristicas de X" es bueno

### Evitar Bucles
1. **No pidas respuestas largas**: Maximo 2000 tokens
2. **Usa listas**: Evita parrafos largos
3. **Se directo**: Una pregunta por prompt
4. **Usa skills**: Los system prompts evitan repeticiones

### Verificar Informacion
1. **Pide fuentes**: "Segun que fuente?"
2. **Marca dudas**: "¿Esto es verificable?"
3. **Compara**: Busca en multiples fuentes
4. **Documenta**: Guarda todo con metadata

---

## Proximos Pasos

1. **Probar los skills** con diferentes tipos de investigacion
2. **Configurar webbridge** para busquedas web
3. **Crear base de datos** de referencias visuales
4. **Documentar hallazgos** en guia de estilo

---

## Notas

- El modelo local funciona mejor con prompts cortos y especificos
- Los skills evitan la mayoria de problemas de bucles
- La busqued web requiere webbridge configurado
- Toda la informacion debe verificarse con fuentes primarias