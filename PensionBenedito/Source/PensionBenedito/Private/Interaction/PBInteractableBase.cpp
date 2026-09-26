// Copyright Pension Benedito, 2024. All rights reserved.

#include "Interaction/PBInteractableBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "PensionBenedito.h"

APBInteractableBase::APBInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    // Interaction sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetSphereRadius(150.0f);
    InteractionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    InteractionSphere->SetGenerateOverlapEvents(true);

    // Prompt widget
    PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PromptWidget"));
    PromptWidget->SetupAttachment(RootComponent);
    PromptWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    PromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
    PromptWidget->SetDrawAtDesiredSize(true);
    PromptWidget->SetVisibility(false);

    // Default settings
    InteractionPromptText = FText::FromString("Press E to interact");
    ObjectName = FText::FromString("Object");
    bCanBeInteractedWith = true;
    bShowPromptWhenFocused = true;
    bIsFocused = false;
}

void APBInteractableBase::BeginPlay()
{
    Super::BeginPlay();

    // Hide prompt initially
    UpdatePromptVisibility(false);
}

bool APBInteractableBase::CanInteract(AActor* Interactor) const
{
    if (!bCanBeInteractedWith)
    {
        return false;
    }

    return CheckInteractionConditions(Interactor);
}

void APBInteractableBase::OnInteract(AActor* Interactor)
{
    if (!CanInteract(Interactor))
    {
        UE_LOG(LogPBInteraction, Warning, TEXT("Cannot interact with %s"), *GetName());
        return;
    }

    // Call Blueprint implementation
    OnInteraction(Interactor);

    UE_LOG(LogPBInteraction, Log, TEXT("Interacted with %s"), *GetName());
}

FText APBInteractableBase::GetInteractionPrompt() const
{
    if (bCanBeInteractedWith)
    {
        return InteractionPromptText;
    }

    return FText::GetEmpty();
}

void APBInteractableBase::SetInteractionEnabled(bool bEnabled)
{
    bCanBeInteractedWith = bEnabled;

    if (!bEnabled)
    {
        UpdatePromptVisibility(false);
    }
}

void APBInteractableBase::UpdatePromptVisibility(bool bVisible)
{
    if (PromptWidget && bShowPromptWhenFocused)
    {
        PromptWidget->SetVisibility(bVisible);
    }
}

bool APBInteractableBase::CheckInteractionConditions(AActor* Interactor) const
{
    // Override in subclasses for custom conditions
    // Default: always can interact
    return true;
}