// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Components/AttributeSetBase.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "PensionBenedito.h"

UAttributeSetBase::UAttributeSetBase()
{
    // Initialize default values
    InitHealth(100.0f);
    InitMaxHealth(100.0f);
    InitStamina(100.0f);
    InitMaxStamina(100.0f);
    InitSanity(100.0f);
    InitMaxSanity(100.0f);
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // Clamp health to valid range
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetStaminaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
    }
    else if (Attribute == GetSanityAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSanity());
    }
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // Handle health changes (damage/healing)
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        // Check for death
        if (GetHealth() <= 0.0f)
        {
            UE_LOG(LogPB, Log, TEXT("Character %s has died"), *Data.Target.GetActor()->GetName());
            // Death handling will be done via Gameplay Effects or Blueprint
        }
    }

    // Handle stamina changes
    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
    }

    // Handle sanity changes
    if (Data.EvaluatedData.Attribute == GetSanityAttribute())
    {
        SetSanity(FMath::Clamp(GetSanity(), 0.0f, GetMaxSanity()));

        // Trigger sanity effects when below threshold
        if (GetSanity() < 30.0f)
        {
            UE_LOG(LogPBTerror, Warning, TEXT("Character %s has low sanity: %.1f"),
                *Data.Target.GetActor()->GetName(), GetSanity());
        }
    }
}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Sanity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxSanity, COND_None, REPNOTIFY_Always);
}

void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
}

void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Stamina, OldStamina);
}

void UAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxStamina, OldMaxStamina);
}

void UAttributeSetBase::OnRep_Sanity(const FGameplayAttributeData& OldSanity)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Sanity, OldSanity);
}

void UAttributeSetBase::OnRep_MaxSanity(const FGameplayAttributeData& OldMaxSanity)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxSanity, OldMaxSanity);
}