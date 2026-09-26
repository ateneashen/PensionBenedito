// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue system for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NAKDialogueNode.generated.h"

class UTexture2D;

/**
 * Represents a single dialogue choice presented to the player.
 * Each choice links to a next dialogue node and can display a tooltip.
 */
USTRUCT(BlueprintType)
struct NARRATIVEACTIONKIT_API FNAKDialogueChoice
{
	GENERATED_BODY()

	/** Text displayed for this choice */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText ChoiceText;

	/** The dialogue node to advance to when this choice is selected */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* NextNode;

	/** Optional tooltip text explaining the consequences or context of this choice */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText ChoiceTooltip;

	FNAKDialogueChoice()
		: NextNode(nullptr)
	{
	}
};

/**
 * A single node in a dialogue tree. Represents one line of dialogue
 * (speaker text + optional player choices) in a narrative conversation.
 *
 * Create instances as Data Assets in the editor. Link them together
 * via NextNode (linear) or Choices (branching) to build full conversations.
 */
UCLASS(BlueprintType)
class NARRATIVEACTIONKIT_API UNAKDialogueNode : public UDataAsset
{
	GENERATED_BODY()

public:
	UNAKDialogueNode();

	// --- Configuration ---

	/** Name of the character speaking this line */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText SpeakerName;

	/** Portrait texture displayed alongside the dialogue (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	UTexture2D* SpeakerPortrait;

	/** The dialogue text spoken by the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText DialogueText;

	/** If true, this node represents a player dialogue line (player responses, not NPC speech) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	bool bIsPlayerNode;

	/** If true, this node ends the conversation when reached */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	bool bIsEndNode;

	/** Next node for linear (non-branching) dialogue flow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* NextNode;

	/** Available choices when this node has branching dialogue */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	TArray<FNAKDialogueChoice> Choices;

	// --- Accessors ---

	/** Get the dialogue text for this node */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	FText GetDialogueText() const;

	/** Get the speaker's display name */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	FText GetSpeakerName() const;

	/** Get the next node (linear flow). Returns nullptr if this is an end node or has choices */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* GetNextNode() const;

	/** Get the available choices for this node */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	const TArray<FNAKDialogueChoice>& GetChoices() const;

	/** Returns true if this node presents multiple choices to the player */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	bool HasChoices() const;

	/** Returns true if this is a terminal node (conversation ends here) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	bool IsEndNode() const;
};