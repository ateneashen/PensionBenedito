// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/NAKCharacterBase.h"
#include "NAKPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNAKDialogueComponent;

/**
 * Player character for NarrativeActionKit.
 * Handles input (classic UE4.27), camera, and player-specific systems.
 * 
 * This is a GENERIC player character for third-person narrative games.
 * For game-specific logic, inherit and extend.
 */
UCLASS()
class NARRATIVEACTIONKIT_API ANAKPlayerCharacter : public ANAKCharacterBase
{
    GENERATED_BODY()

public:
    ANAKPlayerCharacter();

    // Camera components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // Dialogue component (player-specific)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKDialogueComponent* DialogueComponent;

    // Get the player controller
    UFUNCTION(BlueprintPure, Category = "Player")
    APlayerController* GetNAKPlayerController() const;

    // Dialogue state
    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool IsInDialogue() const { return bIsInDialogue; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // Input functions (virtual for game-specific override)
    virtual void MoveForward(float Value);
    virtual void MoveRight(float Value);
    virtual void TurnAtRate(float Rate);
    virtual void LookUpAtRate(float Rate);
    virtual void Interact();
    virtual void OpenInventory();
    virtual void PauseGame();

    // Input properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    float BaseTurnRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    float BaseLookUpRate;

    // Interaction state
    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    bool bIsInDialogue;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetDialogueState(bool bInDialogue);

    // Movement restrictions during dialogue
    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
};