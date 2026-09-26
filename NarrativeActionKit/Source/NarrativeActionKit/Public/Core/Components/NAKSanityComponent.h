// Copyright (c) 2024 NarrativeActionKit. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NAKSanityComponent.generated.h"

class AActor;

/**
 * Sanity levels for horror/mystery games.
 * Thresholds: Stable(>80), Uneasy(>60), Disturbed(>40), Unstable(>20), Breaking(>threshold), Broken(<=threshold).
 */
UENUM(BlueprintType)
enum class ENakSanityLevel : uint8
{
	/** Sanity > 80 - Character is mentally stable */
	Stable     UMETA(DisplayName = "Stable"),
	/** Sanity > 60 - Minor unease, subtle effects */
	Uneasy     UMETA(DisplayName = "Uneasy"),
	/** Sanity > 40 - Noticeable mental disturbance */
	Disturbed  UMETA(DisplayName = "Disturbed"),
	/** Sanity > 20 - Significant instability */
	Unstable   UMETA(DisplayName = "Unstable"),
	/** Sanity > BreakThreshold - On the verge of breaking */
	Breaking   UMETA(DisplayName = "Breaking"),
	/** Sanity <= BreakThreshold - Mental breakdown */
	Broken     UMETA(DisplayName = "Broken")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityChanged, float, NewSanity, ENakSanityLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSanityLevelChanged, ENakSanityLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSanityBroken);

/**
 * UNAKSanityComponent - Reusable sanity management for horror/mystery games.
 *
 * This component is project-agnostic. It manages a sanity value [0-100],
 * broadcasts delegates on changes, and provides a virtual ApplySanityEffects()
 * that games override to apply their own visual/audio effects.
 *
 * Usage:
 *   1. Add this component to your Character or relevant Actor.
 *   2. Configure SanityRecoveryRate, SanityDrainRate, SanityBreakThreshold in defaults.
 *   3. Call ModifySanity() / ApplyHorrorEvent() from gameplay code.
 *   4. Create a Blueprint subclass and override ApplySanityEffects() for visual feedback.
 *   5. Bind to OnSanityChanged / OnSanityLevelChanged / OnSanityBroken as needed.
 */
UCLASS(ClassGroup=(NarrativeActionKit), meta=(BlueprintSpawnableComponent))
class NARRATIVEACTIONKIT_API UNAKSanityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNAKSanityComponent();

	// --- Delegates ---

	/** Fired every time the sanity value changes. */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Sanity")
	FOnSanityChanged OnSanityChanged;

	/** Fired when the sanity level bracket changes (e.g. Stable -> Uneasy). */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Sanity")
	FOnSanityLevelChanged OnSanityLevelChanged;

	/** Fired once when sanity drops to or below SanityBreakThreshold for the first time. */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Sanity")
	FOnSanityBroken OnSanityBroken;

	// --- Configuration ---

	/** Rate at which sanity recovers per tick interval when not at max. Units per second. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NarrativeActionKit|Sanity", meta = (ClampMin = "0.0"))
	float SanityRecoveryRate;

	/** Rate at which sanity drains per tick interval (passive drain). Units per second. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NarrativeActionKit|Sanity", meta = (ClampMin = "0.0"))
	float SanityDrainRate;

	/** Sanity level at or below which the character is considered "Broken". */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NarrativeActionKit|Sanity", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float SanityBreakThreshold;

	/** Interval in seconds between recovery ticks. 0 disables passive recovery. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NarrativeActionKit|Sanity", meta = (ClampMin = "0.0"))
	float SanityRecoveryInterval;

	/** Whether passive sanity recovery is enabled. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NarrativeActionKit|Sanity")
	bool bEnablePassiveRecovery;

	// --- Public API ---

	/**
	 * Modify sanity by a signed amount. Positive values restore, negative values drain.
	 * Clamped to [0, 100]. Broadcasts OnSanityChanged and OnSanityLevelChanged if level bracket changes.
	 * @param Amount  Signed delta to apply to current sanity.
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Sanity")
	void ModifySanity(float Amount);

	/**
	 * Set sanity to an absolute value. Clamped to [0, 100].
	 * @param NewSanity  The target sanity value.
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Sanity")
	void SetSanity(float NewSanity);

	/**
	 * Get the current raw sanity value.
	 * @return Current sanity in [0, 100].
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Sanity")
	float GetSanity() const;

	/**
	 * Get the current sanity level bracket.
	 * @return The ENakSanityLevel corresponding to current sanity.
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Sanity")
	ENakSanityLevel GetSanityLevel() const;

	/**
	 * Check if sanity has reached or fallen below the break threshold.
	 * @return true if the character is in Broken state.
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Sanity")
	bool IsSanityBroken() const;

	/**
	 * Get sanity as a normalized percentage [0.0, 1.0].
	 * @return Sanity divided by 100.
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Sanity")
	float GetSanityPercentage() const;

	/**
	 * Apply a horror event that drains sanity proportional to severity.
	 * Severity is multiplied by SanityDrainRate to scale the effect.
	 * @param Severity  Intensity multiplier (1.0 = standard event, 2.0 = severe, etc.).
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Sanity")
	void ApplyHorrorEvent(float Severity);

	/**
	 * Explicitly recover sanity by a given amount.
	 * @param Amount  Positive amount to restore.
	 */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Sanity")
	void RecoverSanity(float Amount);

	/**
	 * Get a human-readable name for the given sanity level.
	 * @param Level  The level to describe.
	 * @return Display name string.
	 */
	UFUNCTION(BlueprintPure, Category = "NarrativeActionKit|Sanity")
	static FText GetSanityLevelDisplayName(ENakSanityLevel Level);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Override in subclasses (Blueprint or C++) to apply visual/audio effects
	 * based on the current sanity level. Called whenever the level bracket changes.
	 *
	 * This is the primary extension point for games using this framework component.
	 * @param NewLevel  The sanity level that was just entered.
	 * @param PreviousLevel  The sanity level that was just left.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "NarrativeActionKit|Sanity")
	void ApplySanityEffects(ENakSanityLevel NewLevel, ENakSanityLevel PreviousLevel);
	virtual void ApplySanityEffects_Implementation(ENakSanityLevel NewLevel, ENakSanityLevel PreviousLevel);

private:
	/** Current sanity value [0, 100]. */
	float CurrentSanity;

	/** Track the last level to detect bracket transitions. */
	ENakSanityLevel LastSanityLevel;

	/** Whether OnSanityBroken has already been fired. Prevents repeated broadcasts. */
	bool bHasBroken;

	/** Accumulator for recovery tick interval. */
	float RecoveryTimer;

	/** Internal: calculate the level for a given sanity value. */
	ENakSanityLevel CalculateLevel(float SanityValue) const;

	/** Internal: apply sanity change and broadcast delegates as needed. */
	void InternalSetSanity(float NewSanity);
};