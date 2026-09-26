// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBDialogueNode.generated.h"

class UPBDialogueCondition;
class UPBDialogueEvent;

USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ChoiceText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPBDialogueNode* NextNode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UPBDialogueCondition*> Conditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ChoiceTooltip;

    FDialogueChoice()
        : NextNode(nullptr)
    {}
};

/**
 * A single node in a dialogue tree.
 * Contains speaker info, dialogue text, choices, and events.
 * Supports branching conversations with conditions and consequences.
 */
UCLASS(BlueprintType)
class PENSIONBENEDITO_API UPBDialogueNode : public UDataAsset
{
    GENERATED_BODY()

public:
    UPBDialogueNode();

    // Speaker information
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speaker")
    FText SpeakerName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speaker")
    UTexture2D* SpeakerPortrait;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speaker")
    FName SpeakerID;

    // Dialogue content
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
    FText DialogueText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
    FText DialogueTextES; // Spanish translation

    // Node properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node")
    bool bIsPlayerNode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node")
    bool bIsEndNode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node")
    float AutoAdvanceDelay;

    // Next node (for linear dialogue)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation")
    UPBDialogueNode* NextNode;

    // Choices (for branching dialogue)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation")
    TArray<FDialogueChoice> Choices;

    // Conditions to show this node
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Conditions")
    TArray<UPBDialogueCondition*> NodeConditions;

    // Events to execute when node is shown
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Events")
    TArray<UPBDialogueEvent*> NodeEvents;

    // Get dialogue text (supports localization)
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FText GetDialogueText() const;

    // Get speaker name
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FText GetSpeakerName() const { return SpeakerName; }

    // Get next node (linear)
    UFUNCTION(BlueprintPure, Category = "Navigation")
    UPBDialogueNode* GetNextNode() const { return NextNode; }

    // Get choices
    UFUNCTION(BlueprintPure, Category = "Navigation")
    TArray<FDialogueChoice> GetChoices() const { return Choices; }

    // Check if all conditions are met
    UFUNCTION(BlueprintCallable, Category = "Conditions")
    bool CheckConditions(AActor* Context) const;

    // Execute all events
    UFUNCTION(BlueprintCallable, Category = "Events")
    void ExecuteEvents(AActor* Context);

    // Has choices?
    UFUNCTION(BlueprintPure, Category = "Navigation")
    bool HasChoices() const { return Choices.Num() > 0; }

    // Is this the end of the conversation?
    UFUNCTION(BlueprintPure, Category = "Node")
    bool IsEndNode() const { return bIsEndNode; }
};