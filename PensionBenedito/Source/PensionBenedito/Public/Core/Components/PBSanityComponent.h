// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBSanityComponent.generated.h"

UENUM(BlueprintType)
enum class ESanityLevel : uint8
{
    Stable      UMETA(DisplayName = "Stable"),
    Uneasy      UMETA(DisplayName = "Uneasy"),
    Disturbed   UMETA(DisplayName = "Disturbed"),
    Unstable    UMETA(DisplayName = "Unstable"),
    Breaking    UMETA(DisplayName = "Breaking"),
    Broken      UMETA(DisplayName = "Broken")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityChanged, float, NewSanity, ESanityLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSanityLevelChanged, ESanityLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSanityBroken);

/**
 * Sanity component for the terror system.
 * Manages the player's mental state based on horrific events.
 * Affects gameplay, visuals, and audio through GAS effects.
 * Inspired by Bloodborne's insight system and Lovecraftian themes.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PENSIONBENEDITO_API UPBSanityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPBSanityComponent();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Sanity")
    FOnSanityChanged OnSanityChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sanity")
    FOnSanityLevelChanged OnSanityLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sanity")
    FOnSanityBroken OnSanityBroken;

    // Sanity settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sanity")
    float SanityRecoveryRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sanity")
    float SanityDrainRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sanity")
    float SanityBreakThreshold;

    // Modify sanity
    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void ModifySanity(float Amount);

    // Set sanity directly
    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void SetSanity(float NewSanity);

    // Get current sanity
    UFUNCTION(BlueprintPure, Category = "Sanity")
    float GetSanity() const { return CurrentSanity; }

    // Get sanity level
    UFUNCTION(BlueprintPure, Category = "Sanity")
    ESanityLevel GetSanityLevel() const { return CurrentLevel; }

    // Is sanity broken?
    UFUNCTION(BlueprintPure, Category = "Sanity")
    bool IsSanityBroken() const { return CurrentSanity <= SanityBreakThreshold; }

    // Get sanity percentage (0-1)
    UFUNCTION(BlueprintPure, Category = "Sanity")
    float GetSanityPercentage() const;

    // Apply horror event (immediate sanity loss)
    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void ApplyHorrorEvent(float Severity);

    // Recover sanity (rest, safe areas, etc.)
    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void RecoverSanity(float Amount);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    float CurrentSanity;

    UPROPERTY()
    ESanityLevel CurrentLevel;

    // Update sanity level based on current value
    void UpdateSanityLevel();

    // Apply sanity effects (visual, audio, GAS)
    void ApplySanityEffects();

    // Get level for sanity value
    ESanityLevel CalculateSanityLevel(float SanityValue) const;
};