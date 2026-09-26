// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/PBCharacterBase.h"
#include "PBNPCCharacter.generated.h"

class UPBDialogueTree;
class UPBDialogueComponent;

/**
 * NPC character for Pension Benedito.
 * Supports dialogue, quests, and interactive behaviors.
 * Represents residents, visitors, and mysterious figures in the pensions.
 * Period setting: 1930s Spain with appropriate mannerisms and speech.
 */
UCLASS()
class PENSIONBENEDITO_API APBNPCCharacter : public APBCharacterBase
{
    GENERATED_BODY()

public:
    APBNPCCharacter();

    // Dialogue component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UPBDialogueComponent* DialogueComponent;

    // NPC settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UPBDialogueTree* DefaultDialogue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UPBDialogueTree* GreetingDialogue;

    // NPC state
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    bool bHasBeenSpokenTo;

    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    bool bIsInConversation;

    // Start dialogue with player
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool StartConversation(AActor* PlayerCharacter);

    // End conversation
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void EndConversation();

    // Get NPC name
    UFUNCTION(BlueprintPure, Category = "NPC")
    FText GetNPCName() const { return NPCName; }

    // Get dialogue tree (can be overridden for dynamic dialogues)
    UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
    UPBDialogueTree* GetDialogueTree() const;
    virtual UPBDialogueTree* GetDialogueTree_Implementation() const;

protected:
    virtual void BeginPlay() override;

    // Called when dialogue ends
    UFUNCTION()
    void OnDialogueEnded(UPBDialogueTree* EndedTree);

    // Override to add custom greeting logic
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void OnConversationStarted(AActor* PlayerCharacter);

    // Override to add custom farewell logic
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void OnConversationEnded(AActor* PlayerCharacter);
};