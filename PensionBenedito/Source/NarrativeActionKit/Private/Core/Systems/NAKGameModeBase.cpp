// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Systems/NAKGameModeBase.h"
#include "Core/Systems/NAKNarrativeManager.h"
#include "NarrativeActionKit.h"

ANAKGameModeBase::ANAKGameModeBase()
{
    // Create narrative manager
    NarrativeManager = CreateDefaultSubobject<UNAKNarrativeManager>(TEXT("NarrativeManager"));

    // Default values
    CurrentLocation = NAME_None;
    CurrentChapter = 1;
    SaveSlotName = TEXT("NarrativeActionKit_Save");
}

void ANAKGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    InitializeGame();
}

void ANAKGameModeBase::InitializeGame()
{
    UE_LOG(LogNAK, Log, TEXT("Initializing NarrativeActionKit game..."));

    // Initialize narrative flags
    InitializeNarrativeFlags();

    // Initialize systems
    if (NarrativeManager)
    {
        NarrativeManager->Initialize();
    }

    UE_LOG(LogNAK, Log, TEXT("Game initialized - Location: %s, Chapter: %d"),
        *CurrentLocation.ToString(), CurrentChapter);
}

void ANAKGameModeBase::ChangeLocation(FName NewLocation)
{
    if (CurrentLocation == NewLocation)
    {
        return;
    }

    FName OldLocation = CurrentLocation;
    CurrentLocation = NewLocation;

    UE_LOG(LogNAK, Log, TEXT("Location changed: %s -> %s"), *OldLocation.ToString(), *NewLocation.ToString());

    // Notify Blueprint
    OnLocationChanged(NewLocation);

    // Save game
    SaveGameState();
}

void ANAKGameModeBase::ProgressChapter()
{
    CurrentChapter++;

    UE_LOG(LogNAK, Log, TEXT("Chapter progressed to: %d"), CurrentChapter);

    // Notify Blueprint
    OnChapterProgressed(CurrentChapter);

    // Save game
    SaveGameState();
}

void ANAKGameModeBase::SaveGameState()
{
    // Override in game-specific class
    UE_LOG(LogNAK, Log, TEXT("Game saved"));
}

void ANAKGameModeBase::LoadGameState()
{
    // Override in game-specific class
    UE_LOG(LogNAK, Log, TEXT("Game loaded"));
}

void ANAKGameModeBase::InitializeNarrativeFlags()
{
    // Override in game-specific class to set initial flags
    UE_LOG(LogNAKNarrative, Log, TEXT("Narrative flags initialized"));
}