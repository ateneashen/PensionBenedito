// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.generated.h"

// Macro to generate attribute accessors
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Base attribute set for Pension Benedito.
 * Contains core attributes shared by all characters.
 * Extend this for game-specific attributes (combat, sanity, etc.)
 */
UCLASS()
class PENSIONBENEDITO_API UAttributeSetBase : public UAttributeSet
{
    GENERATED_BODY()

public:
    UAttributeSetBase();

    // Core Attributes - Health
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Health")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth)

    // Core Attributes - Stamina (for actions, dodging)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Stamina")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, Stamina)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Stamina")
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxStamina)

    // Core Attributes - Sanity (terror system)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Sanity, Category = "Sanity")
    FGameplayAttributeData Sanity;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, Sanity)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSanity, Category = "Sanity")
    FGameplayAttributeData MaxSanity;
    ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxSanity)

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