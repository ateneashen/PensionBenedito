// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NAKAttributeSet.generated.h"

// Macro to generate attribute accessors
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Base attribute set for NarrativeActionKit.
 * Contains core attributes shared by all narrative action games.
 * 
 * Attributes:
 * - Health: Character's life force
 * - Stamina: Energy for actions (dodging, running, abilities)
 * - Sanity: Mental stability (for horror games)
 * 
 * Extend this for game-specific attributes (Mana, Shield, etc.)
 */
UCLASS()
class NARRATIVEACTIONKIT_API UNAKAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UNAKAttributeSet();

    // Core Attributes - Health
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Health")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, MaxHealth)

    // Core Attributes - Stamina
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Stamina")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Stamina")
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, MaxStamina)

    // Core Attributes - Sanity (for horror games)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Sanity, Category = "Sanity")
    FGameplayAttributeData Sanity;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, Sanity)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSanity, Category = "Sanity")
    FGameplayAttributeData MaxSanity;
    ATTRIBUTE_ACCESSORS(UNAKAttributeSet, MaxSanity)

    // Attribute change callbacks
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth);
    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
    UFUNCTION()
    void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
    UFUNCTION()
    void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
    UFUNCTION()
    void OnRep_Sanity(const FGameplayAttributeData& OldSanity);
    UFUNCTION()
    void OnRep_MaxSanity(const FGameplayAttributeData& OldMaxSanity);
};