// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PBGameMode.generated.h"

class UPBNarrativeManager;

/**
 * Game Mode for Pension Benedito.
 * Manages game state, narrative progression, and system initialization.
 * Handles both Madrid and Barcelona pension scenarios.
 */
UCLASS()
class PENSIONBENEDITO_API APBGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    APBGameMode();

    // Narrative manager
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    UPBNarrativeManager* NarrativeManager;

    // Current location
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    FName CurrentLocation;

    // Game progression
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 CurrentChapter;

    // Initialize game systems
    UFUNCTION(BlueprintCallable, Category = "Game")
    void InitializeGame();

    // Change location (Madrid/Barcelona)
    UFUNCTION(BlueprintCallable, Category = "Game")
    void ChangeLocation(FName NewLocation);

    // Progress to next chapter
    UFUNCTION(BlueprintCallable, Category = "Game")
    void ProgressChapter();

    // Save game state
    UFUNCTION(BlueprintCallable, Category = "Game")
    void SaveGameState();

    // Load game state
    UFUNCTION(BlueprintCallable, Category = "Game")
    void LoadGameState();

protected:
    virtual void BeginPlay() override;

    // Called when location changes
    UFUNCTION(BlueprintImplementableEvent, Category = "Game")
    void OnLocationChanged(FName NewLocation);

    // Called when chapter progresses
    UFUNCTION(BlueprintImplementableEvent, Category = "Game")
    void OnChapterProgressed(int32 NewChapter);

private:
    // Game save slot
    UPROPERTY()
    FString SaveSlotName;

    // Initialize narrative flags
    void InitializeNarrativeFlags();
};