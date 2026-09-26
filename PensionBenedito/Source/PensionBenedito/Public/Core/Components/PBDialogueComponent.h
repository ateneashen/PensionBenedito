// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBDialogueComponent.generated.h"

class UPBDialogueTree;
class UPBDialogueNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, UPBDialogueTree*, DialogueTree);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEnded, UPBDialogueTree*, DialogueTree);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueNodeChanged, UPBDialogueNode*, NewNode, int32, NodeIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceMade, int32, ChoiceIndex);

/**
 * Dialogue component for managing conversations.
 * Supports branching dialogues, conditions, events, and multiple speakers.
 * Integrates with the narrative system for story progression.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PENSIONBENEDITO_API UPBDialogueComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPBDialogueComponent();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueEnded OnDialogueEnded;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueNodeChanged OnDialogueNodeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueChoiceMade OnDialogueChoiceMade;

    // Start a dialogue
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool StartDialogue(UPBDialogueTree* DialogueTree);

    // End current dialogue
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void EndDialogue();

    // Progress to next node
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void ProgressDialogue();

    // Make a choice (for branching dialogues)
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void MakeChoice(int32 ChoiceIndex);

    // Is dialogue active?
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool IsDialogueActive() const { return bIsDialogueActive; }

    // Get current node
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    UPBDialogueNode* GetCurrentNode() const { return CurrentNode; }

    // Get current speaker name
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FText GetCurrentSpeakerName() const;

    // Get current dialogue text
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FText GetCurrentDialogueText() const;

    // Get available choices
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    TArray<FText> GetCurrentChoices() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UPBDialogueTree* CurrentDialogueTree;

    UPROPERTY()
    UPBDialogueNode* CurrentNode;

    UPROPERTY()
    int32 CurrentNodeIndex;

    UPROPERTY()
    bool bIsDialogueActive;

    // Process dialogue node events
    void ProcessNodeEvents(UPBDialogueNode* Node);

    // Check node conditions
    bool CheckNodeConditions(UPBDialogueNode* Node) const;
};