// Copyright 2024 NarrativeActionKit. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NAKNarrativeManager.generated.h"

/**
 * Narrative state data structure for save/load functionality
 */
USTRUCT(BlueprintType)
struct NARRATIVEACTIONKIT_API FNAKNarrativeState
{
	GENERATED_BODY()

	/** Boolean flags for tracking narrative state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TMap<FName, bool> Flags;

	/** Currently active quests */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TSet<FName> ActiveQuests;

	/** Completed quests */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TSet<FName> CompletedQuests;

	/** Failed quests */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TSet<FName> FailedQuests;

	/** Recorded discoveries */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TSet<FName> Discoveries;

	/** Collected clues with descriptions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit")
	TMap<FName, FText> Clues;
};

/**
 * Delegate for flag changes
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlagChanged, FName, FlagName, bool, NewValue);

/**
 * Delegate for quest updates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, FName, QuestID);

/**
 * Core narrative manager that handles flags, quests, discoveries, and clues
 * for the NarrativeActionKit framework
 */
UCLASS(BlueprintType, Blueprintable)
class NARRATIVEACTIONKIT_API UNAKNarrativeManager : public UObject
{
	GENERATED_BODY()

public:
	UNAKNarrativeManager();

	/**
	 * Initialize the narrative manager
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit")
	void Initialize();

	// ===== Flag Functions =====

	/**
	 * Set a narrative flag
	 * @param FlagName - Name of the flag to set
	 * @param bValue - Value to set
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Flags")
	void SetFlag(FName FlagName, bool bValue);

	/**
	 * Get a narrative flag value
	 * @param FlagName - Name of the flag to get
	 * @return Current flag value (false if not set)
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Flags")
	bool GetFlag(FName FlagName) const;

	/**
	 * Check if a narrative flag exists
	 * @param FlagName - Name of the flag to check
	 * @return True if the flag exists
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Flags")
	bool HasFlag(FName FlagName) const;

	// ===== Quest Functions =====

	/**
	 * Start a new quest
	 * @param QuestID - Identifier for the quest to start
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Quests")
	void StartQuest(FName QuestID);

	/**
	 * Complete a quest
	 * @param QuestID - Identifier for the quest to complete
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Quests")
	void CompleteQuest(FName QuestID);

	/**
	 * Fail a quest
	 * @param QuestID - Identifier for the quest to fail
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Quests")
	void FailQuest(FName QuestID);

	/**
	 * Check if a quest is currently active
	 * @param QuestID - Identifier for the quest to check
	 * @return True if the quest is active
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Quests")
	bool IsQuestActive(FName QuestID) const;

	/**
	 * Check if a quest has been completed
	 * @param QuestID - Identifier for the quest to check
	 * @return True if the quest is completed
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Quests")
	bool IsQuestCompleted(FName QuestID) const;

	// ===== Discovery Functions =====

	/**
	 * Record a discovery
	 * @param DiscoveryID - Identifier for the discovery
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Discoveries")
	void RecordDiscovery(FName DiscoveryID);

	/**
	 * Check if a discovery has been made
	 * @param DiscoveryID - Identifier for the discovery to check
	 * @return True if the discovery has been recorded
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Discoveries")
	bool HasDiscovery(FName DiscoveryID) const;

	// ===== Clue Functions =====

	/**
	 * Add a clue to the collection
	 * @param ClueID - Identifier for the clue
	 * @param Description - Description of the clue
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Clues")
	void AddClue(FName ClueID, const FText& Description);

	/**
	 * Get all collected clues
	 * @return Map of clue IDs to descriptions
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Clues")
	TMap<FName, FText> GetCollectedClues() const;

	// ===== Save/Load Functions =====

	/**
	 * Save the current narrative state
	 * @return Current narrative state
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|SaveLoad")
	FNAKNarrativeState SaveState() const;

	/**
	 * Load a narrative state
	 * @param State - Narrative state to load
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|SaveLoad")
	void LoadState(const FNAKNarrativeState& State);

	// ===== Progress Functions =====

	/**
	 * Get overall narrative progress (0-1)
	 * @return Progress value between 0 and 1
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit")
	float GetNarrativeProgress() const;

	// ===== Delegates =====

	/** Called when a flag changes */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Events")
	FOnFlagChanged OnFlagChanged;

	/** Called when a quest is updated */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Events")
	FOnQuestUpdated OnQuestUpdated;

protected:
	/** Current narrative state */
	FNAKNarrativeState NarrativeState;

private:
	/** Total number of narrative elements tracked for progress calculation */
	int32 TotalNarrativeElements;
};