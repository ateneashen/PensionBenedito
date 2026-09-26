// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games (adventure, horror, mystery)

#pragma once

#include "CoreMinimal.h"

// Log categories for NarrativeActionKit systems
DECLARE_LOG_CATEGORY_EXTERN(LogNAK, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKCombat, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKDialogue, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKInteraction, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKNarrative, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKTerror, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKInventory, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNAKAudio, Log, All);

// Console variable for interaction debug visualization
extern TAutoConsoleVariable<int32> CVarNAKInteractionDebug;