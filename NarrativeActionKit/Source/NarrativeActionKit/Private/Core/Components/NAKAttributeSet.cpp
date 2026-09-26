// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Components/NAKAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "NarrativeActionKit.h"

UNAKAttributeSet::UNAKAttributeSet()
{
    // Initialize default values
    InitHealth(100.0f);
    InitMaxHealth(100.0f);
    InitStamina(100.0f);
    InitMaxStamina(100.0f);
    InitSanity(100.0f);
    InitMaxSanity(100.0f);
}

void UNAKAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

void UNAKAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // Handle health changes (damage/healing)
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        // Check for death
        if (GetHealth() <= 0.0f)
        {
            UE_LOG(LogNAK, Log, TEXT("Character %s has died"), *Data.Target.GetActor()->GetName());
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
            UE_LOG(LogNAKTerror, Warning, TEXT("Character %s has low sanity: %.1f"),
                *Data.Target.GetActor()->GetName(), GetSanity());
        }
    }
}

void UNAKAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, Sanity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNAKAttributeSet, MaxSanity, COND_None, REPNOTIFY_Always);
}

void UNAKAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, Health, OldHealth);
}

void UNAKAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, MaxHealth, OldMaxHealth);
}

void UNAKAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, Stamina, OldStamina);
}

void UNAKAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, MaxStamina, OldMaxStamina);
}

void UNAKAttributeSet::OnRep_Sanity(const FGameplayAttributeData& OldSanity)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, Sanity, OldSanity);
}

void UNAKAttributeSet::OnRep_MaxSanity(const FGameplayAttributeData& OldMaxSanity)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNAKAttributeSet, MaxSanity, OldMaxSanity);
}