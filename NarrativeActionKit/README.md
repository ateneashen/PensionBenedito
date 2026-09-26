# NarrativeActionKit — Reusable Framework for Narrative Action Games

## Overview

**NarrativeActionKit** is a reusable Unreal Engine 4.27.2 framework for creating narrative-driven action games with elements of horror, mystery, and adventure. Designed to be project-agnostic, it provides a complete set of modular systems that can be used across multiple games.

### Games Made with This Framework:
- **Pensión Benedito** — Horror mystery in 1930s Spain
- *[Your next project]* — Any narrative action game

---

## Architecture

### Design Principles

1. **Project-Aagnostic Core** — All core systems are generic and reusable
2. **Game-Specific Extensions** — Each game inherits and extends the core
3. **Data-Driven Design** — Configure behavior through DataAssets, not code
4. **Modular Components** — Mix and match systems as needed
5. **GAS Integration** — Gameplay Ability System for attributes and effects

### Module Structure

```
NarrativeActionKit/
├── Source/
│   └── NarrativeActionKit/
│       ├── Public/                    # Headers (API)
│       │   ├── Core/                  # Core systems (always included)
│       │   │   ├── Character/         # Base character classes
│       │   │   ├── Components/        # Reusable components
│       │   │   ├── Systems/           # Game systems (GameMode, etc.)
│       │   │   └── DataAssets/        # Base data asset classes
│       │   ├── Interaction/           # Interaction system
│       │   ├── Dialogue/              # Dialogue system
│       │   ├── Inventory/             # Inventory system
│       │   ├── Narrative/             # Narrative/quest system
│       │   ├── Terror/                # Horror/sanity system
│       │   ├── Combat/                # Combat system (optional)
│       │   ├── UI/                    # UI components
│       │   └── Audio/                 # Audio system
│       │
│       └── Private/                   # Implementations
│           └── [mirrors Public structure]
│
├── Content/
│   ├── Blueprints/                    # Base Blueprint classes
│   ├── DataAssets/                    # Template DataAssets
│   └── UI/                            # Base UI widgets
│
├── Config/                            # Configuration files
└── Documentation/                     # This file
```

---

## Core Systems

### 1. Character System

**Base Classes:**
- `ANAKCharacterBase` — Abstract character with GAS, interaction, inventory
- `ANAKPlayerCharacter` — Player character with input, camera, dialogue
- `ANAKNPCCharacter` — NPC with dialogue, AI behavior

**Features:**
- GAS integration (attributes, abilities, effects)
- Component-based architecture
- Interaction detection
- Sanity/terror system

### 2. Component System

**Reusable Components:**
- `UNAKInteractionComponent` — Detect and execute interactions
- `UNAKInventoryComponent` — Item management with slots
- `UNAKDialogueComponent` — Dialogue management
- `UNAKSanityComponent` — Sanity/terror system
- `UNAKAttributeSet` — GAS attributes (Health, Stamina, Sanity)

**Design:**
- Each component is independent
- Can be added to any Actor
- Configurable via UPROPERTY

### 3. Interaction System

**Classes:**
- `ANAKInteractableBase` — Base for all interactable objects
- `ANAKDocument` — Readable documents/notes
- `ANAKPickupItem` — Collectible items
- `ANAKDoor` — Doors with locks
- `ANAKLever` — Switches and levers

**Features:**
- Raycast detection
- Interaction prompts
- Event-driven callbacks
- Blueprint-extensible

### 4. Dialogue System

**Classes:**
- `UNAKDialogueTree` — DataAsset containing dialogue nodes
- `UNAKDialogueNode` — Individual dialogue with choices
- `UNAKDialogueCondition` — Conditions for showing nodes
- `UNAKDialogueEvent` — Events triggered during dialogue

**Features:**
- Branching conversations
- Condition-based branching
- Events (give items, set flags, modify sanity)
- Localization support

### 5. Inventory System

**Classes:**
- `UNAKItemDefinition` — DataAsset defining item properties
- `UNAKInventoryComponent` — Component managing items

**Features:**
- Slot-based inventory
- Item stacking
- Item types (documents, keys, tools, consumables)
- Delegates for UI updates

### 6. Narrative System

**Classes:**
- `UNAKNarrativeManager` — Manages story state
- `UNAKQuestDefinition` — Quest DataAsset
- `FNARRarrativeFlag` — Serializable flags

**Features:**
- Narrative flags (boolean state)
- Quest tracking (active, completed, failed)
- Discovery system
- Clue collection
- Save/Load support

### 7. Terror/Sanity System

**Classes:**
- `UNAKSanityComponent` — Sanity management

**Features:**
- Sanity levels (Stable → Broken)
- Horror events (immediate sanity loss)
- Recovery mechanics
- Visual/audio effect hooks
- GAS integration for debuffs

### 8. Combat System (Optional)

**Classes:**
- `ANAKWeaponBase` — Base weapon class
- `ANAKCombatComponent` — Combat logic
- `ANAKEnemyBase` — Base enemy with AI

**Features:**
- Melee combat
- Hit detection
- Damage system via GAS
- Enemy AI (behavior trees)

---

## How to Use

### Option 1: Use as Module Dependency

1. Copy `NarrativeActionKit` folder to your project's `Source/` directory
2. Add to your `.Build.cs`:
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[]
   {
       "NarrativeActionKit",
       "GameplayAbilities",
       "GameplayTags",
       "GameplayTasks"
   });
   ```
3. Include headers: `#include "Core/Character/NAKCharacterBase.h"`

### Option 2: Inherit and Extend

1. Create your game module (e.g., `PensionBenedito`)
2. Add `NarrativeActionKit` as dependency
3. Create game-specific classes inheriting from NAK base classes:

```cpp
// PensionBeneditoCharacter.h
#include "Core/Character/NAKPlayerCharacter.h"

class APBPlayerCharacter : public ANAKPlayerCharacter
{
    GENERATED_BODY()
    
    // Add game-specific functionality
    // Override virtual functions
    // Add game-specific components
};
```

### Option 3: Use Components Only

1. Add `NarrativeActionKit` as dependency
2. Add components to your existing actors:

```cpp
// In your Actor's constructor
InteractionComponent = CreateDefaultSubobject<UNAKInteractionComponent>(TEXT("Interaction"));
InventoryComponent = CreateDefaultSubobject<UNAKInventoryComponent>(TEXT("Inventory"));
```

---

## Configuration

### Gameplay Tags

Define your game's tags in `Config/DefaultGameplayTags.ini`:

```ini
[/Script/GameplayTags.GameplayTagsSettings]
+GameplayTagList=(Tag="State.Dead",DevComment="Character is dead")
+GameplayTagList=(Tag="State.InDialogue",DevComment="Character is in dialogue")
+GameplayTagList=(Tag="Item.Key",DevComment="Key item")
```

### Input Mappings

Configure input in `Config/DefaultInput.ini`:

```ini
-AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)
+AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)
+AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=S)
+ActionMappings=(ActionName="Interact",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=E)
```

---

## Extension Points

### Creating Game-Specific Systems

1. **Inherit from base classes:**
   ```cpp
   class APBGameMode : public ANAKGameModeBase
   {
       // Add Pension Benedito specific logic
   };
   ```

2. **Override virtual functions:**
   ```cpp
   virtual void OnDialogueEnded(UNAKDialogueTree* Tree) override;
   ```

3. **Add new components:**
   ```cpp
   UPROPERTY(VisibleAnywhere)
   UPBLetterSystem* LetterSystem;
   ```

### Creating Custom DataAssets

1. **Inherit from base DataAsset:**
   ```cpp
   UCLASS()
   class UPBItemDefinition : public UNAKItemDefinition
   {
       // Add game-specific item properties
   };
   ```

2. **Create in Editor:**
   - Right-click in Content Browser
   - Miscellaneous → Data Asset
   - Select your custom class

---

## Best Practices

### Code Organization

1. **One class per file** — Easy to find and maintain
2. **Public/Private split** — Headers in Public, implementations in Private
3. **Prefix naming** — `NAK` for framework, `PB` for Pension Benedito
4. **Component-based** — Prefer components over deep inheritance

### Performance

1. **Disable tick when not needed** — `PrimaryComponentTick.bCanEverTick = false`
2. **Use timers over tick** — `GetWorldTimerManager().SetTimer()`
3. **Async loading** — Use `TSoftObjectPtr` for large assets
4. **Object pooling** — Reuse actors instead of spawn/destroy

### GAS Best Practices

1. **Attributes in PlayerState** — Survive respawn
2. **Effects over direct modification** — Use GameplayEffects
3. **Tags for state** — Use GameplayTags for character state
4. **Minimal replication** — Only replicate what's needed

---

## Example: Creating a New Game

### Step 1: Create Project Structure

```
MyNewGame/
├── Source/
│   └── MyNewGame/
│       ├── MyNewGame.Build.cs          # Add NarrativeActionKit dependency
│       ├── MyNewGame.h/.cpp
│       ├── Character/
│       │   ├── MNGPlayerCharacter.h    # Inherits ANAKPlayerCharacter
│       │   └── MNGNPCCharacter.h       # Inherits ANAKNPCCharacter
│       └── Systems/
│           └── MNGGameMode.h           # Inherits ANAKGameModeBase
├── Content/
│   ├── Blueprints/
│   ├── DataAssets/
│   └── Maps/
└── Config/
```

### Step 2: Inherit from Base Classes

```cpp
// MNGPlayerCharacter.h
#pragma once

#include "Core/Character/NAKPlayerCharacter.h"
#include "MNGPlayerCharacter.generated.h"

UCLASS()
class AMNGPlayerCharacter : public ANAKPlayerCharacter
{
    GENERATED_BODY()

public:
    AMNGPlayerCharacter();

    // Add game-specific systems
    UPROPERTY(VisibleAnywhere)
    UMNGSpellSystem* SpellSystem;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
};
```

### Step 3: Create DataAssets

1. Create `BP_PlayerCharacter` Blueprint inheriting from `MNGPlayerCharacter`
2. Create `DA_Sword` DataAsset for a weapon
3. Create `DT_Quest1` DialogueTree for a conversation
4. Create `IT_HealthPotion` ItemDefinition for an item

### Step 4: Configure and Play

1. Set GameMode in World Settings
2. Place player character in level
3. Add interactable objects
4. Test!

---

## Roadmap

### ✅ Phase 1: Core Framework (Complete)
- Character system
- Component system
- Interaction system
- Dialogue system
- Inventory system
- Narrative system
- Terror/Sanity system

### 🚧 Phase 2: Combat System (In Progress)
- Weapon system
- Enemy AI
- Combo system

### 📋 Phase 3: UI System (Planned)
- Dialogue UI
- Inventory UI
- HUD components
- Menu system

### 🎨 Phase 4: Audio System (Planned)
- Ambient audio
- Music system
- Sound effects
- Voice acting support

---

## License

Copyright NarrativeActionKit, 2024. All rights reserved.

Free for use in commercial and non-commercial projects.

---

## Support

For questions, issues, or contributions:
- Check the Documentation folder
- Review example implementations
- Contact the development team