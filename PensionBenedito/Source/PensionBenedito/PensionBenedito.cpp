// Copyright Pension Benedito, 2024. All rights reserved.

#include "PensionBenedito.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, PensionBenedito, "PensionBenedito");

// Define log categories
DEFINE_LOG_CATEGORY(LogPB);
DEFINE_LOG_CATEGORY(LogPBCombat);
DEFINE_LOG_CATEGORY(LogPBDialogue);
DEFINE_LOG_CATEGORY(LogPBInteraction);
DEFINE_LOG_CATEGORY(LogPBNarrative);
DEFINE_LOG_CATEGORY(LogPBTerror);