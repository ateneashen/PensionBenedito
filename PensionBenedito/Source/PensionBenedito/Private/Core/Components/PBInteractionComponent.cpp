// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Components/PBInteractionComponent.h"
#include "Interaction/PBInteractableBase.h"
#include "PensionBenedito.h"

UPBInteractionComponent::UPBInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f; // Check every 100ms

    // Default settings
    InteractionRange = 250.0f;
    InteractionRadius = 30.0f;
    InteractionChannel = ECC_Visibility;
    CurrentInteractable = nullptr;
    bIsInteracting = false;
}

void UPBInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPBInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsInteracting)
    {
        CheckForInteractable();
    }
}

void UPBInteractionComponent::CheckForInteractable()
{
    APBInteractableBase* FoundInteractable = TraceForInteractable();

    if (FoundInteractable != CurrentInteractable)
    {
        // Lost previous interactable
        if (CurrentInteractable)
        {
            CurrentInteractable->OnFocusLost();
            OnInteractableLost.Broadcast();
            UE_LOG(LogPBInteraction, Verbose, TEXT("Lost interactable: %s"), *CurrentInteractable->GetName());
        }

        // Found new interactable
        CurrentInteractable = FoundInteractable;

        if (CurrentInteractable)
        {
            CurrentInteractable->OnFocusGained();
            OnInteractableFound.Broadcast(CurrentInteractable);
            UE_LOG(LogPBInteraction, Verbose, TEXT("Found interactable: %s"), *CurrentInteractable->GetName());
        }
    }
}

APBInteractableBase* UPBInteractionComponent::TraceForInteractable() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    APlayerController* PC = Cast<APlayerController>(Cast<APawn>(Owner)->GetController());
    if (!PC)
    {
        return nullptr;
    }

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraRotation.Vector() * InteractionRange);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.bTraceComplex = false;
    QueryParams.bReturnPhysicalMaterial = false;

    FHitResult HitResult;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        FQuat::Identity,
        InteractionChannel,
        FCollisionShape::MakeSphere(InteractionRadius),
        QueryParams
    );

    if (bHit && HitResult.GetActor())
    {
        return Cast<APBInteractableBase>(HitResult.GetActor());
    }

    return nullptr;
}

bool UPBInteractionComponent::TryInteract()
{
    if (!CurrentInteractable || bIsInteracting)
    {
        return false;
    }

    // Check if interactable can be interacted with
    if (!CurrentInteractable->CanInteract(GetOwner()))
    {
        UE_LOG(LogPBInteraction, Warning, TEXT("Cannot interact with %s"), *CurrentInteractable->GetName());
        return false;
    }

    // Start interaction
    bIsInteracting = true;
    CurrentInteractable->OnInteract(GetOwner());

    // Broadcast completion
    OnInteractionComplete.Broadcast(CurrentInteractable);

    UE_LOG(LogPBInteraction, Log, TEXT("Interacted with %s"), *CurrentInteractable->GetName());

    // Reset interaction state after a delay
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
    {
        bIsInteracting = false;
    }, 0.5f, false);

    return true;
}

FText UPBInteractionComponent::GetInteractionPrompt() const
{
    if (CurrentInteractable)
    {
        return CurrentInteractable->GetInteractionPrompt();
    }

    return FText::GetEmpty();
}