// Copyright (c) 2024 NarrativeActionKit. All Rights Reserved.

#include "Core/Components/NAKSanityComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogNAKTerror, Log, All);

// Threshold constants matching the enum documentation
namespace NAKSanityConstants
{
	static constexpr float MaxSanity = 100.0f;
	static constexpr float MinSanity = 0.0f;
	static constexpr float StableThreshold = 80.0f;
	static constexpr float UneasyThreshold = 60.0f;
	static constexpr float DisturbedThreshold = 40.0f;
	static constexpr float UnstableThreshold = 20.0f;
}

// =============================================================================
// Lifecycle
// =============================================================================

UNAKSanityComponent::UNAKSanityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// Default configuration
	CurrentSanity = NAKSanityConstants::MaxSanity;
	LastSanityLevel = ENakSanityLevel::Stable;
	bHasBroken = false;
	RecoveryTimer = 0.0f;

	SanityRecoveryRate = 2.0f;
	SanityDrainRate = 0.0f;
	SanityBreakThreshold = 10.0f;
	SanityRecoveryInterval = 0.5f;
	bEnablePassiveRecovery = true;
}

void UNAKSanityComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize level tracking
	LastSanityLevel = CalculateLevel(CurrentSanity);
	bHasBroken = (CurrentSanity <= SanityBreakThreshold);

	UE_LOG(LogNAKTerror, Log, TEXT("NAKSanityComponent initialized on [%s] — Sanity: %.1f, Level: %s"),
		*GetOwner()->GetName(), CurrentSanity, *GetSanityLevelDisplayName(LastSanityLevel).ToString());
}

void UNAKSanityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogNAKTerror, Log, TEXT("NAKSanityComponent shutting down on [%s] — Final sanity: %.1f"),
		*GetOwner()->GetName(), CurrentSanity);

	Super::EndPlay(EndPlayReason);
}

// =============================================================================
// Tick — passive recovery
// =============================================================================

void UNAKSanityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnablePassiveRecovery)
	{
		return;
	}

	// Apply passive drain every frame (continuous)
	if (SanityDrainRate > 0.0f)
	{
		const float DrainAmount = SanityDrainRate * DeltaTime;
		InternalSetSanity(FMath::Clamp(CurrentSanity - DrainAmount, NAKSanityConstants::MinSanity, NAKSanityConstants::MaxSanity));
	}

	// Apply recovery on interval
	if (SanityRecoveryRate > 0.0f && SanityRecoveryInterval > 0.0f)
	{
		RecoveryTimer += DeltaTime;
		if (RecoveryTimer >= SanityRecoveryInterval)
		{
			RecoveryTimer -= SanityRecoveryInterval;

			if (CurrentSanity < NAKSanityConstants::MaxSanity)
			{
				const float RecoveryAmount = SanityRecoveryRate * SanityRecoveryInterval;
				InternalSetSanity(FMath::Clamp(CurrentSanity + RecoveryAmount, NAKSanityConstants::MinSanity, NAKSanityConstants::MaxSanity));
			}
		}
	}
}

// =============================================================================
// Public API
// =============================================================================

void UNAKSanityComponent::ModifySanity(float Amount)
{
	const float NewSanity = FMath::Clamp(CurrentSanity + Amount, NAKSanityConstants::MinSanity, NAKSanityConstants::MaxSanity);
	InternalSetSanity(NewSanity);
}

void UNAKSanityComponent::SetSanity(float NewSanity)
{
	InternalSetSanity(FMath::Clamp(NewSanity, NAKSanityConstants::MinSanity, NAKSanityConstants::MaxSanity));
}

float UNAKSanityComponent::GetSanity() const
{
	return CurrentSanity;
}

ENakSanityLevel UNAKSanityComponent::GetSanityLevel() const
{
	return CalculateLevel(CurrentSanity);
}

bool UNAKSanityComponent::IsSanityBroken() const
{
	return CurrentSanity <= SanityBreakThreshold;
}

float UNAKSanityComponent::GetSanityPercentage() const
{
	return FMath::Clamp(CurrentSanity / NAKSanityConstants::MaxSanity, 0.0f, 1.0f);
}

void UNAKSanityComponent::ApplyHorrorEvent(float Severity)
{
	if (Severity <= 0.0f)
	{
		UE_LOG(LogNAKTerror, Warning, TEXT("ApplyHorrorEvent called with non-positive severity: %.2f on [%s]"),
			Severity, *GetOwner()->GetName());
		return;
	}

	// Drain scales with severity: a severity of 1.0 drains SanityDrainRate worth,
	// higher severity multiplies the effect. We use a base of 5.0 as the "standard" drain.
	const float BaseDrain = 5.0f;
	const float DrainAmount = BaseDrain * Severity * (1.0f + SanityDrainRate * 0.1f);

	UE_LOG(LogNAKTerror, Verbose, TEXT("Horror event on [%s] — Severity: %.2f, Drain: %.1f"),
		*GetOwner()->GetName(), Severity, DrainAmount);

	ModifySanity(-DrainAmount);
}

void UNAKSanityComponent::RecoverSanity(float Amount)
{
	if (Amount <= 0.0f)
	{
		UE_LOG(LogNAKTerror, Warning, TEXT("RecoverSanity called with non-positive amount: %.2f on [%s]"),
			Amount, *GetOwner()->GetName());
		return;
	}

	ModifySanity(Amount);
}

FText UNAKSanityComponent::GetSanityLevelDisplayName(ENakSanityLevel Level)
{
	switch (Level)
	{
	case ENakSanityLevel::Stable:    return FText::FromString(TEXT("Stable"));
	case ENakSanityLevel::Uneasy:    return FText::FromString(TEXT("Uneasy"));
	case ENakSanityLevel::Disturbed: return FText::FromString(TEXT("Disturbed"));
	case ENakSanityLevel::Unstable:  return FText::FromString(TEXT("Unstable"));
	case ENakSanityLevel::Breaking:  return FText::FromString(TEXT("Breaking"));
	case ENakSanityLevel::Broken:    return FText::FromString(TEXT("Broken"));
	default:                         return FText::FromString(TEXT("Unknown"));
	}
}

// =============================================================================
// Internal
// =============================================================================

ENakSanityLevel UNAKSanityComponent::CalculateLevel(float SanityValue) const
{
	if (SanityValue > NAKSanityConstants::StableThreshold)
	{
		return ENakSanityLevel::Stable;
	}
	else if (SanityValue > NAKSanityConstants::UneasyThreshold)
	{
		return ENakSanityLevel::Uneasy;
	}
	else if (SanityValue > NAKSanityConstants::DisturbedThreshold)
	{
		return ENakSanityLevel::Disturbed;
	}
	else if (SanityValue > NAKSanityConstants::UnstableThreshold)
	{
		return ENakSanityLevel::Unstable;
	}
	else if (SanityValue > SanityBreakThreshold)
	{
		return ENakSanityLevel::Breaking;
	}
	else
	{
		return ENakSanityLevel::Broken;
	}
}

void UNAKSanityComponent::InternalSetSanity(float NewSanity)
{
	const float OldSanity = CurrentSanity;
	CurrentSanity = FMath::Clamp(NewSanity, NAKSanityConstants::MinSanity, NAKSanityConstants::MaxSanity);

	// Determine if level bracket changed
	const ENakSanityLevel NewLevel = CalculateLevel(CurrentSanity);
	const bool bLevelChanged = (NewLevel != LastSanityLevel);

	// Always broadcast OnSanityChanged when value moves
	if (!FMath::IsNearlyEqual(OldSanity, CurrentSanity, 0.001f))
	{
		OnSanityChanged.Broadcast(CurrentSanity, NewLevel);
	}

	// Broadcast OnSanityLevelChanged on bracket transitions
	if (bLevelChanged)
	{
		UE_LOG(LogNAKTerror, Log, TEXT("Sanity level changed on [%s]: %s -> %s (Sanity: %.1f)"),
			*GetOwner()->GetName(),
			*GetSanityLevelDisplayName(LastSanityLevel).ToString(),
			*GetSanityLevelDisplayName(NewLevel).ToString(),
			CurrentSanity);

		OnSanityLevelChanged.Broadcast(NewLevel);

		// Apply effects via virtual function
		ApplySanityEffects(NewLevel, LastSanityLevel);

		LastSanityLevel = NewLevel;
	}

	// Fire OnSanityBroken once when threshold is crossed
	if (!bHasBroken && CurrentSanity <= SanityBreakThreshold)
	{
		bHasBroken = true;

		UE_LOG(LogNAKTerror, Warning, TEXT("SANITY BROKEN on [%s] — Sanity: %.1f (Threshold: %.1f)"),
			*GetOwner()->GetName(), CurrentSanity, SanityBreakThreshold);

		OnSanityBroken.Broadcast();
	}

	// Reset broken flag if sanity recovers above threshold
	if (bHasBroken && CurrentSanity > SanityBreakThreshold)
	{
		bHasBroken = false;
	}
}

// =============================================================================
// ApplySanityEffects — default no-op, override in subclass
// =============================================================================

void UNAKSanityComponent::ApplySanityEffects_Implementation(ENakSanityLevel NewLevel, ENakSanityLevel PreviousLevel)
{
	// Default implementation does nothing.
	// Games override this in Blueprint subclasses or C++ derived classes to apply:
	//   - Post-process material parameter changes (vignette, desaturation, chromatic aberration)
	//   - Audio effects (heartbeat, whispers, ambient distortion)
	//   - Camera shake or FOV manipulation
	//   - UI effects (HUD distortion, sanity meter color shifts)
	//   - Gameplay modifiers (aim sway, input jitter)
	//
	// Example Blueprint usage:
	//   Switch on NewLevel -> Set Post Process Material Instance Parameter "VignetteIntensity"
	//   For Broken: Play camera shake, trigger jumpscare montage

	UE_LOG(LogNAKTerror, Verbose, TEXT("ApplySanityEffects default (no-op) on [%s]: %s -> %s"),
		*GetOwner()->GetName(),
		*GetSanityLevelDisplayName(PreviousLevel).ToString(),
		*GetSanityLevelDisplayName(NewLevel).ToString());
}