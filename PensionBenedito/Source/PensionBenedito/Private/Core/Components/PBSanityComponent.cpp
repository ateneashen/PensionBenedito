// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Components/PBSanityComponent.h"
#include "Core/Components/AttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "Core/Character/PBCharacterBase.h"
#include "PensionBenedito.h"

UPBSanityComponent::UPBSanityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.5f; // Update every 500ms

    // Default settings
    SanityRecoveryRate = 1.0f; // Per second
    SanityDrainRate = 0.5f;    // Per second in dark areas
    SanityBreakThreshold = 10.0f;

    CurrentSanity = 100.0f;
    CurrentLevel = ESanityLevel::Stable;
}

void UPBSanityComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize from GAS attributes if available
    APBCharacterBase* Owner = Cast<APBCharacterBase>(GetOwner());
    if (Owner && Owner->GetAbilitySystemComponent())
    {
        UAttributeSetBase* AttributeSet = Owner->GetAttributeSet();
        if (AttributeSet)
        {
            CurrentSanity = AttributeSet->GetSanity();
            UpdateSanityLevel();
        }
    }
}

void UPBSanityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Natural recovery (if not in a horror area)
    // TODO: Check if in horror area via tags
    if (CurrentSanity < 100.0f && CurrentSanity > SanityBreakThreshold)
    {
        // Slow recovery
        ModifySanity(SanityRecoveryRate * DeltaTime);
    }
}

void UPBSanityComponent::ModifySanity(float Amount)
{
    float OldSanity = CurrentSanity;
    CurrentSanity = FMath::Clamp(CurrentSanity + Amount, 0.0f, 100.0f);

    if (CurrentSanity != OldSanity)
    {
        // Update GAS attribute
        APBCharacterBase* Owner = Cast<APBCharacterBase>(GetOwner());
        if (Owner && Owner->GetAbilitySystemComponent())
        {
            UAttributeSetBase* AttributeSet = Owner->GetAttributeSet();
            if (AttributeSet)
            {
                AttributeSet->SetSanity(CurrentSanity);
            }
        }

        // Update level
        ESanityLevel OldLevel = CurrentLevel;
        UpdateSanityLevel();

        // Broadcast changes
        OnSanityChanged.Broadcast(CurrentSanity, CurrentLevel);

        if (CurrentLevel != OldLevel)
        {
            OnSanityLevelChanged.Broadcast(CurrentLevel);
            UE_LOG(LogPBTerror, Log, TEXT("Sanity level changed to: %s"), *UEnum::GetValueAsString(CurrentLevel));
        }

        // Check for break
        if (CurrentSanity <= SanityBreakThreshold && OldSanity > SanityBreakThreshold)
        {
            OnSanityBroken.Broadcast();
            UE_LOG(LogPBTerror, Warning, TEXT("SANITY BROKEN!"));
        }

        // Apply effects
        ApplySanityEffects();
    }
}

void UPBSanityComponent::SetSanity(float NewSanity)
{
    ModifySanity(NewSanity - CurrentSanity);
}

float UPBSanityComponent::GetSanityPercentage() const
{
    return CurrentSanity / 100.0f;
}

void UPBSanityComponent::ApplyHorrorEvent(float Severity)
{
    // Immediate sanity loss based on severity
    float SanityLoss = -Severity * 10.0f; // Severity 1-10
    ModifySanity(SanityLoss);

    UE_LOG(LogPBTerror, Log, TEXT("Horror event applied: Severity %.1f, Sanity loss: %.1f"), Severity, SanityLoss);
}

void UPBSanityComponent::RecoverSanity(float Amount)
{
    ModifySanity(Amount);
}

void UPBSanityComponent::UpdateSanityLevel()
{
    CurrentLevel = CalculateSanityLevel(CurrentSanity);
}

ESanityLevel UPBSanityComponent::CalculateSanityLevel(float SanityValue) const
{
    if (SanityValue > 80.0f)
        return ESanityLevel::Stable;
    else if (SanityValue > 60.0f)
        return ESanityLevel::Uneasy;
    else if (SanityValue > 40.0f)
        return ESanityLevel::Disturbed;
    else if (SanityValue > 20.0f)
        return ESanityLevel::Unstable;
    else if (SanityValue > SanityBreakThreshold)
        return ESanityLevel::Breaking;
    else
        return ESanityLevel::Broken;
}

void UPBSanityComponent::ApplySanityEffects()
{
    // TODO: Apply visual effects based on sanity level
    // - Distortion effects
    // - Audio changes
    // - Hallucinations
    // - GAS gameplay effects (debuffs)

    APBCharacterBase* Owner = Cast<APBCharacterBase>(GetOwner());
    if (!Owner || !Owner->GetAbilitySystemComponent())
    {
        return;
    }

    // Example: Apply gameplay effect based on sanity level
    // This would be configured in Blueprint/DataAssets
    switch (CurrentLevel)
    {
    case ESanityLevel::Stable:
        // No effects
        break;
    case ESanityLevel::Uneasy:
        // Minor visual noise
        break;
    case ESanityLevel::Disturbed:
        // Audio whispers, minor distortion
        break;
    case ESanityLevel::Unstable:
        // Hallucinations, movement penalties
        break;
    case ESanityLevel::Breaking:
        // Severe effects, near death
        break;
    case ESanityLevel::Broken:
        // Game over or transformation
        break;
    }
}