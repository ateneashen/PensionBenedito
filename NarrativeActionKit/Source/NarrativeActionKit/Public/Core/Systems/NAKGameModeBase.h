// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NAKGameModeBase.generated.h"

class UNAKNarrativeManager;

/**
 * Base Game Mode for NarrativeActionKit.
 * Manages game state, narrative progression, and system initialization.
 * 
 * This is a GENERIC game mode for narrative games.
 * For game-specific logic, inherit and extend.
 */
UCLASS(Abstract)
class NARRATIVEACTIONKIT_API ANAKGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANAKGameModeBase();

    // Narrative manager
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    UNAKNarrativeManager* NarrativeManager;

    // Game state
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    FName CurrentLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 CurrentChapter;

    // Initialize game systems
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void InitializeGame();

    // Change location
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void ChangeLocation(FName NewLocation);

    // Progress to next chapter
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void ProgressChapter();

    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void SaveGameState();

    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void LoadGameState();

protected:
    virtual void BeginPlay() override;

    // Blueprint events
    UFUNCTION(BlueprintImplementableEvent, Category = "Game")
    void OnLocationChanged(FName NewLocation);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game")
    void OnChapterProgressed(int32 NewChapter);

    // Game save slot
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save")
    FString SaveSlotName;

    // Initialize narrative flags (override in game-specific class)
    virtual void InitializeNarrativeFlags();
};