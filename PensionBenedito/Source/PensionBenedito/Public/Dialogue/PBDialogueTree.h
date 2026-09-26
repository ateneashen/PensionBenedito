// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBDialogueTree.generated.h"

class UPBDialogueNode;

/**
 * Dialogue tree asset containing all nodes for a conversation.
 * Used by the dialogue system to manage branching conversations.
 * Supports multiple speakers, conditions, and narrative events.
 */
UCLASS(BlueprintType)
class PENSIONBENEDITO_API UPBDialogueTree : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPBDialogueTree();

    // Tree metadata
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metadata")
    FName DialogueID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metadata")
    FText DialogueName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metadata")
    FText Description;

    // Nodes in this dialogue tree
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodes")
    TArray<UPBDialogueNode*> Nodes;

    // Starting node
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodes")
    UPBDialogueNode* StartingNode;

    // Get all nodes
    UFUNCTION(BlueprintPure, Category = "Nodes")
    TArray<UPBDialogueNode*> GetNodes() const { return Nodes; }

    // Get starting node
    UFUNCTION(BlueprintPure, Category = "Nodes")
    UPBDialogueNode* GetStartingNode() const { return StartingNode; }

    // Find node by speaker ID
    UFUNCTION(BlueprintPure, Category = "Nodes")
    UPBDialogueNode* FindNodeBySpeaker(FName SpeakerID) const;

    // Get the asset type
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

    // Validate dialogue tree
    UFUNCTION(BlueprintCallable, Category = "Validation")
    bool ValidateDialogueTree() const;
};