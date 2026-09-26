# Pensión Benedito — Framework de Desarrollo

## Visión General

**Pensión Benedito** es un framework de desarrollo para juegos de suspense, misterio y terror, inspirado en referencias como Bloodborne y DMC (Ninja Theory). Diseñado para Unreal Engine 4.27.2, proporciona un conjunto modular de sistemas reutilizables para crear experiencias narrativas inmersivas.

### Contexto del Juego

- **Época**: Años previos a la Guerra Civil Española (1930s)
- **Ubicaciones**: Madrid y Barcelona
- **Premisa**: Dos pensiones "Benedito" aparentemente sin conexión, pero con un vínculo terrorífico
- **Género**: Aventura interactiva con elementos de acción y terror psicológico

---

## Arquitectura del Framework

### Módulos Principales

```
PensionBenedito/
├── Core/
│   ├── Character/
│   │   ├── PBCharacterBase        // Character base con GAS
│   │   ├── PBPlayerCharacter      // Jugador (input, cámara)
│   │   └── PBNPCCharacter         // NPCs interactivos
│   │
│   ├── Components/
│   │   ├── AttributeSetBase       // Atributos GAS (Health, Stamina, Sanity)
│   │   ├── PBInteractionComponent // Sistema de interacción
│   │   ├── PBInventoryComponent   // Sistema de inventario
│   │   ├── PBDialogueComponent    // Sistema de diálogo
│   │   └── PBSanityComponent      // Sistema de terror/sanidad
│   │
│   └── Systems/
│       ├── PBGameMode             // Game Mode principal
│       └── PBNarrativeManager     // Gestión de narrativa
│
├── Interaction/
│   ├── PBInteractableBase         // Objeto interactuable base
│   ├── PBDocument                 // Documentos/Notas
│   └── PBItemDefinition           // Definición de items
│
├── Dialogue/
│   ├── PBDialogueTree             // Árbol de diálogo (DataAsset)
│   ├── PBDialogueNode             // Nodo de diálogo
│   ├── PBDialogueCondition        // Condiciones de diálogo
│   └── PBDialogueEvent            // Eventos de diálogo
│
└── Combat/                        // (Próximamente)
    ├── PBWeaponBase
    ├── PBCombatComponent
    └── PBEnemyBase
```

---

## Sistemas Implementados

### 1. Gameplay Ability System (GAS)

El framework utiliza GAS para manejar atributos y efectos de gameplay:

- **AttributeSetBase**: Atributos compartidos (Health, Stamina, Sanity)
- **Replicación**: Soporte completo para multiplayer
- **Extensible**: Facilidad para añadir atributos específicos del juego

```cpp
// Ejemplo de uso de GAS
UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(HealEffect, 1, EffectContext);
ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
```

### 2. Sistema de Interacción

Detección y ejecución de interacciones con objetos del mundo:

- **Detección por raycast**: Encuentra objetos frente al jugador
- **Prompts contextuales**: Muestra instrucciones al mirar objetos
- **Eventos**: Delegates para UI y lógica de juego

```cpp
// Configurar interacción
InteractionComponent->InteractionRange = 250.0f;
InteractionComponent->OnInteractableFound.AddDynamic(this, &AMyClass::OnFound);
```

### 3. Sistema de Diálogo

Conversaciones ramificadas con opciones múltiples:

- **DataAssets**: Diálogos configurados en el editor
- **Condiciones**: Mostrar opciones según flags narrativos
- **Eventos**: Dar items, modificar sanidad, establecer flags

```cpp
// Iniciar diálogo
DialogueComponent->StartDialogue(DialogueTree);
DialogueComponent->MakeChoice(0); // Seleccionar opción
```

### 4. Sistema de Inventario

Gestión de items con stacking y slots:

- **Tipos de items**: Documentos, llaves, herramientas, consumibles
- **DataAssets**: Definiciones de items reutilizables
- **Delegates**: Notificaciones de cambios

```cpp
// Añadir item
InventoryComponent->AddItem(DocumentItem, 1);
bool hasKey = InventoryComponent->HasKeyItem(KeyItem);
```

### 5. Sistema de Terror/Sanidad

Mecánicas de terror psicológico inspiradas en Bloodborne:

- **Niveles de sanidad**: Stable → Uneasy → Disturbed → Unstable → Breaking → Broken
- **Eventos de horror**: Pérdida de sanidad por eventos terroríficos
- **Efectos progresivos**: Distorsiones visuales, alucinaciones

```cpp
// Aplicar evento de horror
SanityComponent->ApplyHorrorEvent(5.0f); // Severidad 1-10
SanityComponent->RecoverSanity(20.0f);   // Recuperación
```

### 6. Sistema Narrativo

Gestión de la historia y progresión:

- **Flags narrativos**: Variables para tracking de decisiones
- **Quests**: Sistema de misiones
- **Descubrimientos**: Registro de hallazgos
- **Pistas**: Colección de pistas para el misterio

```cpp
// Gestionar narrativa
NarrativeManager->SetFlag(FName("FoundDiary"), true);
NarrativeManager->StartQuest(FName("InvestigatePension"));
NarrativeManager->AddClue(FName("Clue1"), FText::FromString("Una carta sospechosa..."));
```

---

## Guía de Uso

### Configuración Inicial

1. **Importar el módulo** a tu proyecto UE4.27
2. **Configurar dependencias** en tu `.Build.cs`:
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[]
   {
       "PensionBenedito",
       "GameplayAbilities",
       "GameplayTags",
       "GameplayTasks"
   });
   ```
3. **Configurar GAS** en tu GameMode
4. **Crear DataAssets** para items y diálogos

### Crear un NPC Interactivo

1. Crear Blueprint heredando de `PBNPCCharacter`
2. Configurar `DialogueComponent` con un `DialogueTree`
3. Crear `DialogueTree` DataAsset con nodos
4. Añadir condiciones y eventos según necesidad

### Crear un Documento Interactable

1. Crear `ItemDefinition` DataAsset (tipo: Document)
2. Configurar contenido del documento
3. Crear Blueprint heredando de `PBDocument`
4. Asignar `ItemDefinition` al documento
5. Colocar en el mundo

### Crear un Diálogo Ramificado

1. Crear `DialogueTree` DataAsset
2. Añadir nodos con texto y speaker
3. Configurar opciones de elección
4. Añadir condiciones (flags, items)
5. Añadir eventos (dar items, modificar sanidad)

---

## Ejemplo: Interacción con NPC

### Flujo completo:

1. **Jugador mira al NPC** → `OnInteractableFound` se dispara
2. **UI muestra prompt** → "Hablar con [Nombre]"
3. **Jugador pulsa Interact** → `StartConversation()` se llama
4. **Diálogo comienza** → Primer nodo se muestra
5. **Jugador elige opción** → `MakeChoice()` procesa la elección
6. **Eventos se ejecutan** → Items dados, flags modificados
7. **Diálogo termina** → `OnDialogueEnded` se dispara

### Código de ejemplo:

```cpp
// En el NPC Blueprint
void APBNPCCharacter::StartConversation(AActor* PlayerCharacter)
{
    if (DialogueComponent && DefaultDialogue)
    {
        DialogueComponent->StartDialogue(DefaultDialogue);
        bIsInConversation = true;
    }
}

// En el DialogueNode - Evento al llegar al nodo
void UPBEvent_GiveKey::ExecuteEvent_Implementation(AActor* Context)
{
    APBCharacterBase* Character = Cast<APBCharacterBase>(Context);
    if (Character && Character->GetInventoryComponent())
    {
        Character->GetInventoryComponent()->AddItem(KeyItem, 1);
    }
}
```

---

## Extensiones y Personalización

### Añadir Nuevos Atributos

1. Crear nuevo `AttributeSet` heredando de `UAttributeSet`
2. Añadir atributos con `ATTRIBUTE_ACCESSORS` macro
3. Configurar en `PBCharacterBase`

### Añadir Nuevos Tipos de Interacción

1. Heredar de `PBInteractableBase`
2. Override `OnInteraction()` para lógica custom
3. Override `CheckInteractionConditions()` para validación

### Añadir Nuevos Eventos de Diálogo

1. Heredar de `PBDialogueEvent`
2. Override `ExecuteEvent_Implementation()`
3. Añadir propiedades UPROPERTY para configuración

### Añadir Nuevas Condiciones de Diálogo

1. Heredar de `PBDialogueCondition`
2. Override `CheckCondition_Implementation()`
3. Retornar true/false según la condición

---

## Roadmap

### ✅ Fase 1: Core Framework (Completado)
- Character base con GAS
- Sistema de interacción
- Sistema de inventario
- Sistema de diálogo
- Sistema de sanidad
- Sistema narrativo

### 🚧 Fase 2: Combat System (Próximamente)
- Sistema de combate melee
- Armas cuerpo a cuerpo
- Enemigos con IA
- Sistema de combos

### 📋 Fase 3: Puzzle System (Planificado)
- Framework de puzles
- Puzles de lógica
- Puzles de exploración
- Puzles de combinación

### 🎨 Fase 4: Terror/Atmosphere (Planificado)
- Efectos visuales de sanidad
- Sistema de audio ambiental
- Jumpscares controlados
- Iluminación dinámica

---

## Convenciones de Código

- **Prefijos**: PB (Pensión Benedito)
- **Nombres**: PascalCase para clases, camelCase para variables
- **UPROPERTY**: Siempre con Category y Tooltip
- **Delegates**: Desbindeados en EndPlay
- **Logs**: Categoría por sistema (LogPB, LogPBDialogue, etc.)

---

## Licencia

Copyright Pensión Benedito, 2024. All rights reserved.

---

## Contacto

Para preguntas o contribuciones, contactar al equipo de desarrollo.