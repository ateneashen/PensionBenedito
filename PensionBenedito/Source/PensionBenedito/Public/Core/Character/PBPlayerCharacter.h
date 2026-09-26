// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/PBCharacterBase.h"
#include "PBPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPBDialogueComponent;

/**
 * Player character for Pension Benedito.
 * Handles input (classic UE4.27), camera, and player-specific systems.
 * Pre-war Spain setting: third-person perspective with period-appropriate movement.
 */
UCLASS()
class PENSIONBENEDITO_API APBPlayerCharacter : public APBCharacterBase
{
    GENERATED_BODY()

public:
    APBPlayerCharacter();

    // Camera components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // Dialogue component (player-specific)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UPBDialogueComponent* DialogueComponent;

    // Get the player controller
    UFUNCTION(BlueprintPure, Category = "Player")
    APlayerController* GetPBPlayerController() const;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // Input functions
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
    void Interact();
    void OpenInventory();
    void PauseGame();

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