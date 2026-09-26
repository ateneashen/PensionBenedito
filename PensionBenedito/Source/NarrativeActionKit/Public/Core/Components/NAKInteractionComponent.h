// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games (adventure, horror, mystery)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NAKInteractionComponent.generated.h"

// Delegates for interaction events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, AActor*, InteractableActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableLost, AActor*, InteractableActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionComplete, AActor*, InteractableActor, bool, bSuccess);

/**
 * Reusable interaction component for detecting and interacting with actors in range.
 * Uses line trace detection with configurable tick interval for performance.
 * Works with ANY actor - no specific base class required.
 */
UCLASS(ClassGroup=(NarrativeActionKit), meta=(BlueprintSpawnableComponent))
class NARRATIVEACTIONKIT_API UNAKInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNAKInteractionComponent();

	// --- Delegates ---

	/** Fired when an interactable actor enters detection range */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Interaction")
	FOnInteractableFound OnInteractableFound;

	/** Fired when the current interactable actor leaves detection range or is lost */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Interaction")
	FOnInteractableLost OnInteractableLost;

	/** Fired when an interaction attempt completes */
	UPROPERTY(BlueprintAssignable, Category = "NarrativeActionKit|Interaction")
	FOnInteractionComplete OnInteractionComplete;

	// --- Configuration ---

	/** Maximum distance for interaction line trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit|Interaction", meta = (ClampMin = "50.0", ClampMax = "1000.0"))
	float InteractionRange;

	/** Radius for sphere overlap detection (used for initial broad-phase check) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit|Interaction", meta = (ClampMin = "50.0", ClampMax = "500.0"))
	float InteractionRadius;

	/** Interval in seconds between interaction checks (0 = every tick) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit|Interaction", meta = (ClampMin = "0.0", ClampMax = "5.0"))
	float DetectionInterval;

	/** Channel used for line trace detection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit|Interaction")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	/** Whether interaction detection is currently enabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NarrativeActionKit|Interaction")
	bool bInteractionEnabled;

	// --- Functions ---

	/** Attempt to interact with the currently detected interactable actor */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Interaction")
	bool TryInteract();

	/** Manually check for interactable actors in range (called automatically if tick-based detection is active) */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Interaction")
	void CheckForInteractable();

	/** Get the current interaction prompt text from the detected interactable actor */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Interaction")
	FText GetInteractionPrompt() const;

	/** Get the currently detected interactable actor (nullptr if none) */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Interaction")
	AActor* GetCurrentInteractable() const;

	/** Enable or disable interaction detection */
	UFUNCTION(BlueprintCallable, Category = "NarrativeActionKit|Interaction")
	void SetInteractionEnabled(bool bEnable);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The currently detected interactable actor */
	UPROPERTY()
	AActor* CurrentInteractable;

	/** Timer accumulator for detection interval */
	float DetectionTimer;

	/** Perform the actual line trace for interaction detection */
	AActor* PerformInteractionTrace() const;

	/** Update the current interactable reference and fire delegates */
	void SetCurrentInteractable(AActor* NewInteractable);
};