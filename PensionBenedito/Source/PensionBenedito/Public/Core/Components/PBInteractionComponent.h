// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBInteractionComponent.generated.h"

class APBInteractableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, APBInteractableBase*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableLost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionComplete, APBInteractableBase*, Interactable);

/**
 * Component that handles interaction detection and execution.
 * Uses line trace to find interactable objects in front of the character.
 * Supports interaction prompts, input handling, and interaction callbacks.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PENSIONBENEDITO_API UPBInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPBInteractionComponent();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractableFound OnInteractableFound;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractableLost OnInteractableLost;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionComplete OnInteractionComplete;

    // Interaction settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    float InteractionRange;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    float InteractionRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    TEnumAsByte<ECollisionChannel> InteractionChannel;

    // Current interactable
    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    APBInteractableBase* CurrentInteractable;

    // Try to interact with current interactable
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool TryInteract();

    // Check for interactable objects (called from tick or timer)
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void CheckForInteractable();

    // Get interaction prompt text
    UFUNCTION(BlueprintPure, Category = "Interaction")
    FText GetInteractionPrompt() const;

    // Is currently interacting?
    UFUNCTION(BlueprintPure, Category = "Interaction")
    bool IsInteracting() const { return bIsInteracting; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    bool bIsInteracting;

    // Timer for periodic checks
    FTimerHandle CheckTimerHandle;

    // Perform the line trace
    APBInteractableBase* TraceForInteractable() const;
};