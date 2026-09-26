// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue component for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NAKDialogueComponent.generated.h"

class UNAKDialogueTree;
class UNAKDialogueNode;
struct FNAKDialogueChoice;

// Delegates for dialogue events

/** Fired when a dialogue conversation begins */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, UNAKDialogueTree*, DialogueTree);

/** Fired when a dialogue conversation ends */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEnded, UNAKDialogueTree*, DialogueTree);

/** Fired when the current dialogue node changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueNodeChanged, UNAKDialogueNode*, NewNode, int32, NodeIndex);

/** Fired when the player makes a dialogue choice */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceMade, int32, ChoiceIndex);

/**
 * Component that drives dialogue conversations for any actor.
 * Attach to a PlayerController, NPC, or any actor that participates in dialogue.
 *
 * Manages dialogue tree traversal, node navigation, and player choice handling.
 * Fires delegates that UI widgets bind to for displaying dialogue.
 */
UCLASS(ClassGroup=(NarrativeActionKit), meta=(BlueprintSpawnableComponent))
class NARRATIVEACTIONKIT_API UNAKDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNAKDialogueComponent();

	// --- Delegates ---

	/** Fired when a dialogue conversation begins */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Dialogue")
	FOnDialogueStarted OnDialogueStarted;

	/** Fired when a dialogue conversation ends */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Dialogue")
	FOnDialogueEnded OnDialogueEnded;

	/** Fired when the dialogue advances to a new node */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Dialogue")
	FOnDialogueNodeChanged OnDialogueNodeChanged;

	/** Fired when the player selects a dialogue choice */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Dialogue")
	FOnDialogueChoiceMade OnDialogueChoiceMade;

	// --- Functions ---

	/**
	 * Start a dialogue conversation with the given dialogue tree.
	 * Does nothing if a dialogue is already active.
	 * @param DialogueTree  The dialogue tree asset to begin
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Dialogue")
	void StartDialogue(UNAKDialogueTree* DialogueTree);

	/** End the current dialogue conversation. Does nothing if no dialogue is active. */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Dialogue")
	void EndDialogue();

	/**
	 * Advance to the next dialogue node (linear progression).
	 * Does nothing if current node has choices or is an end node.
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Dialogue")
	void ProgressDialogue();

	/**
	 * Select a dialogue choice by index.
	 * Advances the dialogue to the choice's target node.
	 * @param ChoiceIndex  Index into the current node's Choices array
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Dialogue")
	void MakeChoice(int32 ChoiceIndex);

	/** Returns true if a dialogue conversation is currently active */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	bool IsDialogueActive() const;

	/** Get the current dialogue node. Returns nullptr if no dialogue is active. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* GetCurrentNode() const;

	/** Get the speaker name from the current node. Returns empty text if no dialogue is active. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	FText GetCurrentSpeakerName() const;

	/** Get the dialogue text from the current node. Returns empty text if no dialogue is active. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	FText GetCurrentDialogueText() const;

	/** Get available choices from the current node. Returns empty array if no choices or no dialogue is active. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	const TArray<FNAKDialogueChoice>& GetCurrentChoices() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The dialogue tree currently being played */
	UPROPERTY()
	UNAKDialogueTree* CurrentDialogueTree;

	/** The current node in the dialogue tree */
	UPROPERTY()
	UNAKDialogueNode* CurrentNode;

	/** Index of the current node in the dialogue tree's Nodes array */
	int32 CurrentNodeIndex;

	/** Whether a dialogue conversation is currently in progress */
	bool bIsDialogueActive;

	/** Set the current node and fire the node changed delegate */
	void SetCurrentNode(UNAKDialogueNode* NewNode);

	/** Empty choices array returned when no dialogue is active */
	static const TArray<FNAKDialogueChoice> EmptyChoices;
};