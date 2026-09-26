// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/NAKCharacterBase.h"
#include "NAKNPCCharacter.generated.h"

class UNAKDialogueTree;
class UNAKDialogueComponent;

/**
 * NPC character for NarrativeActionKit.
 * Supports dialogue, quests, and interactive behaviors.
 * 
 * This is a GENERIC NPC class for narrative games.
 * For game-specific NPCs, inherit and extend.
 */
UCLASS()
class NARRATIVEACTIONKIT_API ANAKNPCCharacter : public ANAKCharacterBase
{
    GENERATED_BODY()

public:
    ANAKNPCCharacter();

    // Dialogue component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKDialogueComponent* DialogueComponent;

    // NPC settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UNAKDialogueTree* DefaultDialogue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UNAKDialogueTree* GreetingDialogue;

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

    // Get dialogue tree (virtual for dynamic dialogues)
    UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
    UNAKDialogueTree* GetDialogueTree() const;
    virtual UNAKDialogueTree* GetDialogueTree_Implementation() const;

protected:
    virtual void BeginPlay() override;

    // Called when dialogue ends
    UFUNCTION()
    void OnDialogueEnded(UNAKDialogueTree* EndedTree);

    // Blueprint events
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void OnConversationStarted(AActor* PlayerCharacter);

    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void OnConversationEnded(AActor* PlayerCharacter);
};