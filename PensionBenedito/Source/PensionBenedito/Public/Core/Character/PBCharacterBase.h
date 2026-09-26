// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PBCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSetBase;
class UPBInteractionComponent;
class UPBInventoryComponent;
class UPBSanityComponent;

/**
 * Base character class for Pension Benedito.
 * Provides GAS integration, interaction, inventory, and sanity systems.
 * All characters (player and NPCs) inherit from this class.
 */
UCLASS(Abstract)
class PENSIONBENEDITO_API APBCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    APBCharacterBase();

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    /** Get the attribute set for this character */
    UAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

    /** Get the interaction component */
    UPBInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

    /** Get the inventory component */
    UPBInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

    /** Get the sanity component */
    UPBSanityComponent* GetSanityComponent() const { return SanityComponent; }

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UAttributeSetBase* AttributeSet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UPBInteractionComponent* InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UPBInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UPBSanityComponent* SanityComponent;

    // Default abilities to grant on possession
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    // Default effects to apply on possession
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

    /** Grant default abilities and apply effects */
    virtual void InitializeAbilitySystem();

    /** Called when the character dies */
    UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void OnCharacterDeath();
};