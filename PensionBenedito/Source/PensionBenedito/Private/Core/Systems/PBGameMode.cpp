// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Systems/PBGameMode.h"
#include "Core/Systems/PBNarrativeManager.h"
#include "Kismet/GameplayStatics.h"
#include "PensionBenedito.h"

APBGameMode::APBGameMode()
{
    // Create narrative manager
    NarrativeManager = CreateDefaultSubobject<UPBNarrativeManager>(TEXT("NarrativeManager"));

    // Default values
    CurrentLocation = FName("Madrid");
    CurrentChapter = 1;
    SaveSlotName = TEXT("PensionBenedito_Save");
}

void APBGameMode::BeginPlay()
{
    Super::BeginPlay();

    InitializeGame();
}

void APBGameMode::InitializeGame()
{
    UE_LOG(LogPB, Log, TEXT("Initializing Pension Benedito..."));

    // Initialize narrative flags
    InitializeNarrativeFlags();

    // Initialize systems
    if (NarrativeManager)
    {
        NarrativeManager->Initialize();
    }

    UE_LOG(LogPB, Log, TEXT("Game initialized - Location: %s, Chapter: %d"),
        *CurrentLocation.ToString(), CurrentChapter);
}

void APBGameMode::ChangeLocation(FName NewLocation)
{
    if (CurrentLocation == NewLocation)
    {
        return;
    }

    FName OldLocation = CurrentLocation;
    CurrentLocation = NewLocation;

    UE_LOG(LogPB, Log, TEXT("Location changed: %s -> %s"), *OldLocation.ToString(), *NewLocation.ToString());

    // Notify Blueprint
    OnLocationChanged(NewLocation);

    // Save game
    SaveGameState();
}

void APBGameMode::ProgressChapter()
{
    CurrentChapter++;

    UE_LOG(LogPB, Log, TEXT("Chapter progressed to: %d"), CurrentChapter);

    // Notify Blueprint
    OnChapterProgressed(CurrentChapter);

    // Save game
    SaveGameState();
}

void APBGameMode::SaveGameState()
{
    // TODO: Implement save game system
    UE_LOG(LogPB, Log, TEXT("Game saved"));
}

void APBGameMode::LoadGameState()
{
    // TODO: Implement load game system
    UE_LOG(LogPB, Log, TEXT("Game loaded"));
}

void APBGameMode::InitializeNarrativeFlags()
{
    // Initialize default narrative flags
    if (NarrativeManager)
    {
        // Madrid flags
        NarrativeManager->SetFlag(FName("Madrid_IntroComplete"), false);
        NarrativeManager->SetFlag(FName("Madrid_MetOwner"), false);
        NarrativeManager->SetFlag(FName("Madrid_FoundDiary"), false);
        NarrativeManager->SetFlag(FName("Madrid_SolvedPuzzle1"), false);

        // Barcelona flags
        NarrativeManager->SetFlag(FName("Barcelona_IntroComplete"), false);
        NarrativeManager->SetFlag(FName("Barcelona_MetGuest"), false);
        NarrativeManager->SetFlag(FName("Barcelona_FoundLetter"), false);
        NarrativeManager->SetFlag(FName("Barcelona_SolvedPuzzle1"), false);

        // Shared flags
        NarrativeManager->SetFlag(FName("KnowsAboutBenedito"), false);
        NarrativeManager->SetFlag(FName("FoundConnection"), false);
        NarrativeManager->SetFlag(FName("SanityThreshold1"), false);
        NarrativeManager->SetFlag(FName("SanityThreshold2"), false);
    }

    UE_LOG(LogPBNarrative, Log, TEXT("Narrative flags initialized"));
}