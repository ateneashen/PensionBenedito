// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PBNarrativeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlagChanged, FName, FlagName, bool, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, FName, QuestID);

/**
 * Narrative manager for Pension Benedito.
 * Manages story flags, quest progression, and narrative state.
 * Tracks player choices and their consequences across both pensions.
 * Core system for the mystery/horror narrative.
 */
UCLASS(BlueprintType)
class PENSIONBENEDITO_API UPBNarrativeManager : public UObject
{
    GENERATED_BODY()

public:
    UPBNarrativeManager();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Narrative")
    FOnFlagChanged OnFlagChanged;

    UPROPERTY(BlueprintAssignable, Category = "Narrative")
    FOnQuestUpdated OnQuestUpdated;

    // Initialize the manager
    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void Initialize();

    // Flag management
    UFUNCTION(BlueprintCallable, Category = "Flags")
    void SetFlag(FName FlagName, bool Value);

    UFUNCTION(BlueprintPure, Category = "Flags")
    bool GetFlag(FName FlagName) const;

    UFUNCTION(BlueprintPure, Category = "Flags")
    bool HasFlag(FName FlagName) const;

    // Quest management
    UFUNCTION(BlueprintCallable, Category = "Quests")
    void StartQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    void CompleteQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    void FailQuest(FName QuestID);

    UFUNCTION(BlueprintPure, Category = "Quests")
    bool IsQuestActive(FName QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quests")
    bool IsQuestCompleted(FName QuestID) const;

    // Discovery tracking
    UFUNCTION(BlueprintCallable, Category = "Discovery")
    void RecordDiscovery(FName DiscoveryID);

    UFUNCTION(BlueprintPure, Category = "Discovery")
    bool HasDiscovery(FName DiscoveryID) const;

    // Clue system
    UFUNCTION(BlueprintCallable, Category = "Clues")
    void AddClue(FName ClueID, FText ClueDescription);

    UFUNCTION(BlueprintPure, Category = "Clues")
    TArray<FText> GetCollectedClues() const;

    // Narrative state
    UFUNCTION(BlueprintPure, Category = "Narrative")
    float GetNarrativeProgress() const;

    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Save")
    FPNarrativeState SaveState() const;

    UFUNCTION(BlueprintCallable, Category = "Save")
    void LoadState(const FPNarrativeState& State);

protected:
    // Narrative flags
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flags")
    TMap<FName, bool> NarrativeFlags;

    // Active quests
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
    TSet<FName> ActiveQuests;

    // Completed quests
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
    TSet<FName> CompletedQuests;

    // Failed quests
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
    TSet<FName> FailedQuests;

    // Discoveries
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Discovery")
    TSet<FName> Discoveries;

    // Clues
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clues")
    TMap<FName, FText> Clues;
};

/**
 * Serializable narrative state for save/load
 */
USTRUCT(BlueprintType)
struct FPNarrativeState
{
    GENERATED_BODY()

    UPROPERTY()
    TMap<FName, bool> Flags;

    UPROPERTY()
    TSet<FName> Active;

    UPROPERTY()
    TSet<FName> Completed;

    UPROPERTY()
    TSet<FName> Failed;

    UPROPERTY()
    TSet<FName> Discovered;

    UPROPERTY()
    TMap<FName, FText> CluesFound;
};