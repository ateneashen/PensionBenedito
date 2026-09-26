// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBInteractableBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

/**
 * Base class for all interactable objects in Pension Benedito.
 * Provides interaction detection, prompts, and event handling.
 * Used for doors, items, documents, NPCs, and puzzle elements.
 */
UCLASS(Abstract)
class PENSIONBENEDITO_API APBInteractableBase : public AActor
{
    GENERATED_BODY()

public:
    APBInteractableBase();

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* InteractionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* PromptWidget;

    // Interaction settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    FText InteractionPromptText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    FText ObjectName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    bool bCanBeInteractedWith;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    bool bShowPromptWhenFocused;

    // Interaction functions
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual bool CanInteract(AActor* Interactor) const;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void OnInteract(AActor* Interactor);

    // Focus events (when player looks at object)
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnFocusGained();

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnFocusLost();

    // Get interaction prompt
    UFUNCTION(BlueprintPure, Category = "Interaction")
    virtual FText GetInteractionPrompt() const;

    // Get object name
    UFUNCTION(BlueprintPure, Category = "Interaction")
    FText GetObjectName() const { return ObjectName; }

protected:
    virtual void BeginPlay() override;

    // Override in subclasses for custom interaction logic
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteraction(AActor* Interactor);

    // Enable/disable interaction
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetInteractionEnabled(bool bEnabled);

    // Update prompt visibility
    void UpdatePromptVisibility(bool bVisible);

    // Override to add custom conditions
    virtual bool CheckInteractionConditions(AActor* Interactor) const;

private:
    UPROPERTY()
    bool bIsFocused;
};