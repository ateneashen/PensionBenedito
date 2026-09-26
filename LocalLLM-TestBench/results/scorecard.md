# Scorecard — Qwen 3.8 27B

## Resumen

| Categoría | Tests | Promedio | Estado |
|-----------|-------|----------|--------|
| Boilerplate | 1 | 3.75/10 | ⚠️ Requiere supervisión intensiva |
| Refactor | 0 | - | ⏳ Pendiente |
| Documentation | 1 | 7.0/10 | ✅ Delegable con supervisión ligera |
| Complex | 0 | - | ⏳ Pendiente |

## Detalle por test

| # | Categoría | Test | Correctitud | Seguridad | Eficiencia | Usabilidad | Total | Notas |
|---|-----------|------|-------------|-----------|------------|------------|-------|-------|
| 01 | Boilerplate | Health Component | 3/10 | 4/10 | 6/10 | 2/10 | **3.75** | 8 errores de compilación, typos, lógica incorrecta |
| 02 | Documentation | Health Component Docs | 6/10 | - | - | 7/10 | **7.0** | Formato profesional, errores menores de sintaxis Doxygen |

## Observaciones generales

### Fortalezas detectadas
- **Documentación**: Genera docs profesionales con ejemplos útiles de Blueprint
- **Estructura**: Respeta separación .h/.cpp y formato Doxygen
- **Delegates dinámicos**: Conceptualmente correcto (usa la macro adecuada)
- **Logging**: Incluye categorías de log apropiadamente

### Debilidades detectadas
- **Typos frecuentes** en código (DELEGABLE, variable names, etc.)
- **Sintaxis UE4 incorrecta** en macros de delegates (comillas, parámetros)
- **No sigue UPROPERTY conventions** automáticamente (EditAnywhere vs VisibleAnywhere)
- **Inicialización incompleta** en constructores
- **Lógica de negocio** con errores (clamps, validaciones)

### Recomendaciones de delegación

| Tipo de tarea | ¿Delegar? | Supervisión | Ahorro estimado |
|---------------|-----------|-------------|-----------------|
| Documentación/Comentarios | ✅ SÍ | Ligera | 60-70% tokens |
| Boilerplate simple | ⚠️ Con cuidado | Intensiva | 30-40% tokens |
| Refactorización menor | ⚠️ Probar | Media | 40-50% tokens |
| Lógica compleja | ❌ NO | N/A | 0% (riesgo alto) |
| Replicación/Multiplayer | ❌ NO | N/A | 0% (riesgo crítico) |

### Patrón observado

El modelo funciona mejor en:
1. **Tareas de texto natural** (documentación, comentarios)
2. **Generación de estructura** (headers, includes, macros básicas)
3. **Ejemplos y usages** (código de ejemplo para docs)

Y peor en:
1. **Sintaxis precisa** (macros UE4, templates)
2. **Lógica de negocio** (validaciones, edge cases)
3. **Convenciones específicas** (UPROPERTY specifiers, naming)

## Próximos tests recomendados

1. **Test 03**: Refactorización — Agregar `UFUNCTION` a funciones existentes
2. **Test 04**: Generación de interfaz — `UINTERFACE` con `IInteractable`
3. **Test 5**: DataTable struct — `FTableRowBase` con propiedades

---

*Última actualización: 2026-09-26 13:42*