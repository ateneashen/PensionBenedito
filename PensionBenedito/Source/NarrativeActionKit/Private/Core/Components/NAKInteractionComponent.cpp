// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games (adventure, horror, mystery)

#include "Core/Components/NAKInteractionComponent.h"
#include "NarrativeActionKit.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UNAKInteractionComponent::UNAKInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.0f;

	InteractionRange = 300.0f;
	InteractionRadius = 150.0f;
	DetectionInterval = 0.1f;
	InteractionTraceChannel = ECC_Visibility;
	bInteractionEnabled = true;

	CurrentInteractable = nullptr;
	DetectionTimer = 0.0f;
}

void UNAKInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogNAKInteraction, Log, TEXT("NAKInteractionComponent initialized on %s | Range: %.1f | Interval: %.2f"),
		*GetOwner()->GetName(), InteractionRange, DetectionInterval);
}

void UNAKInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bInteractionEnabled)
	{
		return;
	}

	DetectionTimer += DeltaTime;

	if (DetectionTimer >= DetectionInterval)
	{
		DetectionTimer = 0.0f;
		CheckForInteractable();
	}
}

void UNAKInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear current interactable reference
	if (CurrentInteractable)
	{
		OnInteractableLost.Broadcast(CurrentInteractable);
		CurrentInteractable = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

bool UNAKInteractionComponent::TryInteract()
{
	if (!CurrentInteractable)
	{
		UE_LOG(LogNAKInteraction, Warning, TEXT("TryInteract called but no interactable actor detected"));
		OnInteractionComplete.Broadcast(nullptr, false);
		return false;
	}

	// Verify the actor is still valid
	if (!IsValid(CurrentInteractable))
	{
		UE_LOG(LogNAKInteraction, Warning, TEXT("TryInteract: Current interactable is no longer valid"));
		SetCurrentInteractable(nullptr);
		OnInteractionComplete.Broadcast(nullptr, false);
		return false;
	}

	UE_LOG(LogNAKInteraction, Log, TEXT("Interacting with: %s"), *CurrentInteractable->GetName());

	// Call the interaction interface - since we use AActor*, we broadcast the delegate
	// and let the game-specific code handle the actual interaction logic
	OnInteractionComplete.Broadcast(CurrentInteractable, true);

	return true;
}

void UNAKInteractionComponent::CheckForInteractable()
{
	AActor* DetectedActor = PerformInteractionTrace();

	if (DetectedActor != CurrentInteractable)
	{
		SetCurrentInteractable(DetectedActor);
	}
}

FText UNAKInteractionComponent::GetInteractionPrompt() const
{
	if (!CurrentInteractable || !IsValid(CurrentInteractable))
	{
		return FText::GetEmpty();
	}

	// Default implementation returns actor label
	// Game-specific code can bind to OnInteractableFound to set custom prompts
	return FText::FromString(CurrentInteractable->GetActorLabel());
}

AActor* UNAKInteractionComponent::GetCurrentInteractable() const
{
	return CurrentInteractable;
}

void UNAKInteractionComponent::SetInteractionEnabled(bool bEnable)
{
	bInteractionEnabled = bEnable;

	if (!bEnable && CurrentInteractable)
	{
		OnInteractableLost.Broadcast(CurrentInteractable);
		CurrentInteractable = nullptr;
	}

	UE_LOG(LogNAKInteraction, Log, TEXT("Interaction %s on %s"),
		bEnable ? TEXT("enabled") : TEXT("disabled"), *GetOwner()->GetName());
}

AActor* UNAKInteractionComponent::PerformInteractionTrace() const
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// Get player controller for view direction
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		return nullptr;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector TraceStart = ViewLocation;
	FVector TraceEnd = TraceStart + (ViewRotation.Vector() * InteractionRange);

	// Perform line trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult;
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		InteractionTraceChannel,
		QueryParams
	);

#if !UE_BUILD_SHIPPING
	// Debug visualization
	if (CVarNAKInteractionDebug.GetValueOnGameThread())
	{
		DrawDebugLine(World, TraceStart, TraceEnd,
			bHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);

		if (bHit)
		{
			DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 8, FColor::Yellow, false, 0.1f);
		}
	}
#endif

	if (bHit && HitResult.GetActor())
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

void UNAKInteractionComponent::SetCurrentInteractable(AActor* NewInteractable)
{
	// Notify loss of previous interactable
	if (CurrentInteractable && CurrentInteractable != NewInteractable)
	{
		UE_LOG(LogNAKInteraction, Verbose, TEXT("Lost interactable: %s"), *CurrentInteractable->GetName());
		OnInteractableLost.Broadcast(CurrentInteractable);
	}

	// Set new interactable
	CurrentInteractable = NewInteractable;

	// Notify found of new interactable
	if (CurrentInteractable)
	{
		UE_LOG(LogNAKInteraction, Verbose, TEXT("Found interactable: %s"), *CurrentInteractable->GetName());
		OnInteractableFound.Broadcast(CurrentInteractable);
	}
}