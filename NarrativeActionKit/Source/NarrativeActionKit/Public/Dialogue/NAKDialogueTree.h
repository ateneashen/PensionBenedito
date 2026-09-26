// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue system for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Engine/PrimaryDataAsset.h"
#include "NAKDialogueTree.generated.h"

class UNAKDialogueNode;

/**
 * A complete dialogue tree representing a full conversation.
 * Contains all dialogue nodes and manages the conversation structure.
 *
 * Use the Asset Manager to load dialogue trees asynchronously via FPrimaryAssetId
 * ("Dialogue", DialogueID) for efficient memory management in large games.
 */
UCLASS(BlueprintType)
class NARRATIVEACTIONKIT_API UNAKDialogueTree : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UNAKDialogueTree();

	// --- Configuration ---

	/** Unique identifier for this dialogue tree (used with Asset Manager) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FName DialogueID;

	/** Display name for this dialogue (shown in editor) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText DialogueName;

	/** Description of when/where this dialogue is used */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	FText Description;

	/** All dialogue nodes belonging to this tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	TArray<UNAKDialogueNode*> Nodes;

	/** The first node displayed when this dialogue begins */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* StartingNode;

	// --- Accessors ---

	/** Get all dialogue nodes in this tree */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	const TArray<UNAKDialogueNode*>& GetNodes() const;

	/** Get the starting node of this dialogue */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* GetStartingNode() const;

	/** Find a node by speaker name (returns the first match, or nullptr) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NarrativeActionKit|Dialogue")
	UNAKDialogueNode* FindNodeBySpeaker(FName SpeakerID) const;

	/**
	 * Validate the dialogue tree for structural integrity.
	 * Checks: starting node exists, no orphaned nodes, no null next-node references.
	 * Returns true if valid; logs warnings for any issues found.
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Dialogue")
	bool ValidateDialogueTree() const;

	// --- UPrimaryDataAsset ---

	/** Returns PrimaryAssetId of type "Dialogue" with this tree's DialogueID */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};