# NarrativeActionKit — Architecture Guide

## Design Philosophy

### 1. Separation of Concerns

The framework separates **generic systems** (reusable) from **game-specific logic** (customizable):

```
┌─────────────────────────────────────────────────────────┐
│                    GAME-SPECIFIC LAYER                   │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐     │
│  │ Pension     │  │ Your Next   │  │ Horror      │     │
│  │ Benedito    │  │ Game        │  │ Mansion     │     │
│  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘     │
│         │                │                │             │
├─────────┼────────────────┼────────────────┼─────────────┤
│         ▼                ▼                ▼             │
│  ┌─────────────────────────────────────────────────┐   │
│  │              NARRATIVEACTIONKIT                  │   │
│  │  ┌─────────┐ ┌─────────┐ ┌─────────┐           │   │
│  │  │ Core    │ │Dialogue │ │Inventory│  ...       │   │
│  │  └─────────┘ └─────────┘ └─────────┘           │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │              UNREAL ENGINE 4.27                  │   │
│  │  ┌─────────┐ ┌─────────┐ ┌─────────┐           │   │
│  │  │ GAS     │ │UMG      │ │AI       │  ...       │   │
│  │  └─────────┘ └─────────┘ └─────────┘           │   │
│  └─────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

### 2. Component-Based Architecture

Each system is an **independent component** that can be added to any Actor:

```cpp
// Any Actor can use these components
class AMyActor : public AActor
{
    UPROPERTY(VisibleAnywhere)
    UNAKInteractionComponent* Interaction;
    
    UPROPERTY(VisibleAnywhere)
    UNAKInventoryComponent* Inventory;
    
    UPROPERTY(VisibleAnywhere)
    UNAKDialogueComponent* Dialogue;
};
```

**Benefits:**
- Mix and match systems
- Easy to add/remove functionality
- No deep inheritance chains
- Clear separation of responsibilities

### 3. Data-Driven Design

Behavior is configured through **DataAssets**, not hardcoded:

```
DataAssets/
├── ItemDefinitions/          # What items exist
│   ├── IT_Sword.uasset
│   └── IT_HealthPotion.uasset
├── DialogueTrees/            # What NPCs say
│   ├── DT_Merchant.uasset
│   └── DT_QuestGiver.uasset
├── QuestDefinitions/         # What quests exist
│   └── QD_MainQuest1.uasset
└── GameplayEffects/          # What effects do
    ├── GE_Damage.uasset
    └── GE_Heal.uasset
```

**Benefits:**
- Designers can modify without code changes
- Easy to iterate
- No recompilation needed
- Version control friendly

### 4. GAS Integration

The Gameplay Ability System provides:
- **Attributes** — Health, Stamina, Sanity, etc.
- **Effects** — Damage, healing, buffs, debuffs
- **Abilities** — Skills, attacks, spells
- **Tags** — State management

```
┌─────────────────────────────────────────────────────┐
│                  GAS INTEGRATION                     │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ AttributeSet│───▶│ Attributes  │                 │
│  │ (Health,    │    │ (modifiable │                 │
│  │  Stamina,   │    │  via GAS)   │                 │
│  │  Sanity)    │    └─────────────┘                 │
│  └─────────────┘                                    │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Gameplay    │───▶│ Effects     │                 │
│  │ Effects     │    │ (damage,    │                 │
│  │ (GE_*)      │    │  heal, buff)│                 │
│  └─────────────┘    └─────────────┘                 │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Gameplay    │───▶│ Abilities   │                 │
│  │ Abilities   │    │ (skills,    │                 │
│  │ (GA_*)      │    │  attacks)   │                 │
│  └─────────────┘    └─────────────┘                 │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Gameplay    │───▶│ Tags        │                 │
│  │ Tags        │    │ (state,     │                 │
│  │ (State.*)   │    │  flags)     │                 │
│  └─────────────┘    └─────────────┘                 │
└─────────────────────────────────────────────────────┘
```

---

## Class Hierarchy

### Character Hierarchy

```
ACharacter (UE4)
└── ANAKCharacterBase (NAK)
    ├── ANAKPlayerCharacter (NAK)
    │   └── APBPlayerCharacter (Pension Benedito)
    └── ANAKNPCCharacter (NAK)
        └── APBNPCCharacter (Pension Benedito)
```

### Component Hierarchy

```
UActorComponent (UE4)
├── UNAKInteractionComponent (NAK)
├── UNAKInventoryComponent (NAK)
├── UNAKDialogueComponent (NAK)
└── UNAKSanityComponent (NAK)

UAttributeSet (GAS)
└── UNAKAttributeSet (NAK)
```

### DataAsset Hierarchy

```
UPrimaryDataAsset (UE4)
├── UNAKItemDefinition (NAK)
│   └── UPBItemDefinition (Pension Benedito)
├── UNAKDialogueTree (NAK)
│   └── UPBDialogueTree (Pension Benedito)
└── UNAKQuestDefinition (NAK)
    └── UPBQuestDefinition (Pension Benedito)
```

---

## Naming Conventions

### Prefixes

| Prefix | Meaning | Example |
|--------|---------|---------|
| `NAK` | NarrativeActionKit (framework) | `NAKCharacterBase` |
| `PB` | Pension Benedito (game) | `PBPlayerCharacter` |
| `MNG` | My New Game (your game) | `MNGGameMode` |
| `ANAK` | Actor from NAK | `ANAKPlayerCharacter` |
| `UNAK` | UObject from NAK | `UNAKItemDefinition` |
| `FNak` | Struct from NAK | `FNakInventoryItem` |
| `ENak` | Enum from NAK | `ENakSanityLevel` |

### File Naming

| Type | Convention | Example |
|------|------------|---------|
| Character | `{Prefix}{Name}.h` | `NAKPlayerCharacter.h` |
| Component | `{Prefix}{Name}Component.h` | `NAKInteractionComponent.h` |
| DataAsset | `{Prefix}{Name}Definition.h` | `NAKItemDefinition.h` |
| System | `{Prefix}{Name}Manager.h` | `NAKNarrativeManager.h` |

### Property Naming

| Type | Convention | Example |
|------|------------|---------|
| Bool | `b` prefix | `bIsDead`, `bCanInteract` |
| Pointer | No prefix | `InteractionComponent`, `InventoryComponent` |
| Delegate | `On` prefix | `OnHealthChanged`, `OnDialogueEnded` |
| Config | Descriptive | `MaxHealth`, `InteractionRange` |

---

## Module Dependencies

```
NarrativeActionKit
├── Core (always included)
│   ├── Core
│   ├── CoreUObject
│   ├── Engine
│   ├── InputCore
│   └── NavigationSystem
├── GAS (GameplayAbilities)
│   ├── GameplayAbilities
│   ├── GameplayTags
│   └── GameplayTasks
├── UI (optional)
│   └── UMG
└── AI (optional)
    └── AIModule
```

### Adding Dependencies

In your game's `.Build.cs`:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "NarrativeActionKit",  // Always include
    "GameplayAbilities",   // If using GAS
    "GameplayTags",        // If using tags
    "GameplayTasks"        // If using tasks
});
```

---

## Extension Patterns

### Pattern 1: Inheritance

**When to use:** Adding new functionality to existing systems

```cpp
// Base class in NAK
class ANAKPlayerCharacter : public ACharacter
{
    virtual void OnDialogueEnded(UNAKDialogueTree* Tree);
};

// Game-specific override
class APBPlayerCharacter : public ANAKPlayerCharacter
{
    virtual void OnDialogueEnded(UNAKDialogueTree* Tree) override
    {
        Super::OnDialogueEnded(Tree);
        // Add Pension Benedito specific logic
        if (Tree == HorrorDialogue)
        {
            SanityComponent->ApplyHorrorEvent(5.0f);
        }
    }
};
```

### Pattern 2: Composition

**When to use:** Adding new systems without modifying existing classes

```cpp
class APBPlayerCharacter : public ANAKPlayerCharacter
{
    // Add new component
    UPROPERTY(VisibleAnywhere)
    UPBLetterSystem* LetterSystem;
    
    APBPlayerCharacter()
    {
        LetterSystem = CreateDefaultSubobject<UPBLetterSystem>(TEXT("LetterSystem"));
    }
};
```

### Pattern 3: Delegation

**When to use:** Customizing behavior through callbacks

```cpp
// In your game's BeginPlay
void APBPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind to framework delegates
    DialogueComponent->OnDialogueEnded.AddDynamic(
        this, &APBPlayerCharacter::HandleDialogueEnd);
    
    InteractionComponent->OnInteractableFound.AddDynamic(
        this, &APBPlayerCharacter::HandleInteractableFound);
}
```

### Pattern 4: Data Extension

**When to use:** Adding game-specific data to existing DataAssets

```cpp
// Framework base
class UNAKItemDefinition : public UPrimaryDataAsset
{
    FName ItemID;
    FText DisplayName;
    EItemType ItemType;
};

// Game-specific extension
class UPBItemDefinition : public UNAKItemDefinition
{
    // Add Pension Benedito specific properties
    FText DocumentContent;
    FName NarrativeFlag;
    float SanityImpact;
};
```

---

## Save/Load Architecture

### Serialization Strategy

```
┌─────────────────────────────────────────────────────┐
│                  SAVE SYSTEM                         │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Narrative   │───▶│ Flags,      │                 │
│  │ Manager     │    │ Quests,     │                 │
│  │             │    │ Discoveries │                 │
│  └─────────────┘    └─────────────┘                 │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Inventory   │───▶│ Items,      │                 │
│  │ Component   │    │ Quantities  │                 │
│  └─────────────┘    └─────────────┘                 │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ Character   │───▶│ Attributes, │                 │
│  │ State       │    │ Location    │                 │
│  └─────────────┘    └─────────────┘                 │
│                                                     │
│  ┌─────────────┐    ┌─────────────┐                 │
│  │ World       │───▶│ Actor       │                 │
│  │ State       │    │ States      │                 │
│  └─────────────┘    └─────────────┘                 │
└─────────────────────────────────────────────────────┘
```

### Save Game Structure

```cpp
USTRUCT()
struct FNAKSaveGame
{
    // Narrative state
    TMap<FName, bool> NarrativeFlags;
    TSet<FName> ActiveQuests;
    TSet<FName> CompletedQuests;
    
    // Player state
    FVector PlayerLocation;
    FRotator PlayerRotation;
    float Health;
    float Stamina;
    float Sanity;
    
    // Inventory
    TArray<FInventoryItem> InventoryItems;
    
    // World state
    TMap<FName, FActorState> ActorStates;
};
```

---

## Performance Guidelines

### Component Tick

| Component | Default Tick | When to Enable |
|-----------|--------------|----------------|
| Interaction | Enabled (0.1s) | Always (for detection) |
| Inventory | Disabled | Only when UI open |
| Dialogue | Disabled | Only during dialogue |
| Sanity | Enabled (0.5s) | Always (for effects) |

### Memory Management

1. **Use TSoftObjectPtr** for large assets (textures, meshes)
2. **Unload unused assets** when changing areas
3. **Object pooling** for frequently spawned actors
4. **Delegate cleanup** in EndPlay/BeginDestroy

### Replication

| Property | Replicate? | Reason |
|----------|------------|--------|
| Health | Yes | Affects gameplay |
| Inventory | Yes | Shared state |
| Dialogue State | No | Local only |
| Sanity Effects | Partial | Visual only |

---

## Testing Strategy

### Unit Tests

```cpp
// Test inventory system
bool TestInventoryAddItem()
{
    UNAKInventoryComponent* Inventory = NewObject<UNAKInventoryComponent>();
    UNAKItemDefinition* Item = NewObject<UNAKItemDefinition>();
    
    return Inventory->AddItem(Item, 1);
}
```

### Integration Tests

```cpp
// Test dialogue + inventory integration
bool TestDialogueGivesItem()
{
    // Setup
    ANAKNPCCharacter* NPC = SpawnNPC();
    ANAKPlayerCharacter* Player = SpawnPlayer();
    
    // Execute
    NPC->StartConversation(Player);
    Player->GetDialogueComponent()->MakeChoice(0);
    
    // Verify
    return Player->GetInventoryComponent()->HasKeyItem(KeyItem);
}
```

### Blueprint Tests

1. Create test level
2. Place test actors
3. Run automated tests
4. Verify results

---

## Common Patterns

### Pattern: Conditional Dialogue

```
DialogueTree
├── Node 1: "Have you found the key?"
│   ├── Choice A: "Yes" → Node 2 (condition: HasItem(Key))
│   └── Choice B: "No" → Node 3
├── Node 2: "Then open the door" (event: GiveItem(Map))
└── Node 3: "Look in the drawer"
```

### Pattern: Horror Event Sequence

```
1. Player enters dark room
2. SanityComponent->ApplyHorrorEvent(3.0f)
3. Visual effects trigger (blur, distortion)
4. Audio whisper plays
5. Sanity drops below threshold
6. New abilities unlock (GAS)
```

### Pattern: Quest Progression

```
1. Player talks to NPC
2. Quest starts: "Find the diary"
3. Player finds diary (NarrativeManager->RecordDiscovery)
4. Player returns to NPC
5. Quest completes: Reward given
6. New quest unlocks
```

---

## Troubleshooting

### Common Issues

1. **"Cannot find NAK header"**
   - Add `NarrativeActionKit` to `.Build.cs` dependencies
   - Rebuild project

2. **"GAS not working"**
   - Ensure `GameplayAbilities` module is included
   - Check AbilitySystemComponent is created
   - Verify AttributeSet is initialized

3. **"Delegate not firing"**
   - Check delegate is bound in BeginPlay
   - Verify AddDynamic syntax
   - Ensure object is valid

4. **"DataAsset not found"**
   - Check asset path is correct
   - Verify asset is loaded (TSoftObjectPtr)
   - Check primary asset ID

---

## Glossary

| Term | Definition |
|------|------------|
| **NAK** | NarrativeActionKit (this framework) |
| **GAS** | Gameplay Ability System |
| **DataAsset** | UObject containing configuration data |
| **Component** | Reusable functionality module |
| **Delegate** | Callback/event system |
| **Tag** | GameplayTag for state management |
| **Effect** | GameplayEffect (damage, buff, etc.) |
| **Ability** | GameplayAbility (skill, attack) |