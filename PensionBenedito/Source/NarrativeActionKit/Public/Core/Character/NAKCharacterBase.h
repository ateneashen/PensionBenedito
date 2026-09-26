// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "NAKCharacterBase.generated.h"

class UAbilitySystemComponent;
class UNAKAttributeSet;
class UNAKInteractionComponent;
class UNAKInventoryComponent;
class UNAKSanityComponent;

/**
 * Base character class for NarrativeActionKit.
 * Provides GAS integration, interaction, inventory, and sanity systems.
 * All characters (player and NPCs) inherit from this class.
 * 
 * This class is PROJECT-AGNOSTIC — use it in any narrative action game.
 * For game-specific logic, inherit and extend.
 */
UCLASS(Abstract)
class NARRATIVEACTIONKIT_API ANAKCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ANAKCharacterBase();

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    /** Get the attribute set for this character */
    UNAKAttributeSet* GetNAKAttributeSet() const { return AttributeSet; }

    /** Get the interaction component */
    UNAKInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

    /** Get the inventory component */
    UNAKInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

    /** Get the sanity component */
    UNAKSanityComponent* GetSanityComponent() const { return SanityComponent; }

    /** Check if character is alive */
    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsAlive() const;

    /** Get current health */
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const;

    /** Get max health */
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const;

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKAttributeSet* AttributeSet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKInteractionComponent* InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNAKSanityComponent* SanityComponent;

    // Default abilities to grant on possession
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    // Default effects to apply on possession
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

    // Default gameplay tags
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    FGameplayTagContainer DefaultTags;

    /** Initialize ability system (server only) */
    virtual void InitializeAbilitySystem();

    /** Called when the character dies */
    UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void OnCharacterDeath();

    /** Called when health changes */
    UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void OnHealthChanged(float NewHealth, float MaxHealth);
};