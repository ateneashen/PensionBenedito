# Local LLM Test Bench — UE 4.27.2

Banco de pruebas para evaluar la delegación de tareas de desarrollo en Unreal Engine 4.27.2 a un modelo local (Qwen 3.8 27B) ejecutado en llama.cpp.

## Propósito

- **Ahorrar tokens** del modelo de pago delegando tareas repetitivas
- **Evaluar calidad** del código generado por el modelo local
- **Establecer workflow** de supervisión eficiente

## Estructura

```
LocalLLM-TestBench/
├── config.json          # Configuración del modelo y proyecto
├── README.md            # Este archivo
├── tests/               # Tests individuales organizados por categoría
│   ├── 01-boilerplate/  # Generación de código base
│   ├── 02-refactor/     # Refactorización
│   ├── 03-documentation/# Documentación
│   └── 04-complex/      # Lógica compleja (requiere supervisión)
├── results/             # Resultados y evaluaciones
│   └── scorecard.md     # Puntuación acumulada del modelo
└── prompts/             # Prompts utilizados (historial)
```

## Cómo usar

1. Cada test tiene un prompt en `tests/<categoría>/prompt.md`
2. El modelo local genera código
3. Mi supervisión evalúa según los criterios de UE 4.27.2
4. Resultado y puntuación van a `results/`

## Criterios de evaluación

- **Correctitud** (0-10): ¿Compila? ¿Sigue convenciones UE4.27?
- **Seguridad** (0-10): ¿Evita los errores comunes (raw pointers, hard refs, etc.)?
- **Eficiencia** (0-10): ¿Es código limpio y performante?
- **Usabilidad** (0-10): ¿Se puede integrar directamente?

## Modelo bajo prueba

- **Modelo**: Qwen 3.8 27B (GGUF)
- **Endpoint**: http://localhost:8080
- **API Key**: "local"