// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games (adventure, horror, mystery)

#include "NarrativeActionKit.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, NarrativeActionKit, "NarrativeActionKit");

// Define log categories
DEFINE_LOG_CATEGORY(LogNAK);
DEFINE_LOG_CATEGORY(LogNAKCombat);
DEFINE_LOG_CATEGORY(LogNAKDialogue);
DEFINE_LOG_CATEGORY(LogNAKInteraction);
DEFINE_LOG_CATEGORY(LogNAKNarrative);
DEFINE_LOG_CATEGORY(LogNAKTerror);
DEFINE_LOG_CATEGORY(LogNAKInventory);
DEFINE_LOG_CATEGORY(LogNAKAudio);

// Console variable for interaction debug visualization
TAutoConsoleVariable<int32> CVarNAKInteractionDebug(
	TEXT("NAK.Interaction.Debug"),
	0,
	TEXT("Enable debug visualization for NAK Interaction Component\n")
	TEXT("0: Disabled, 1: Enabled"),
	ECVF_Cheat);