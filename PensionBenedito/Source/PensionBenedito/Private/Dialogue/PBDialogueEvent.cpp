// Copyright Pension Benedito, 2024. All rights reserved.

#include "Dialogue/PBDialogueEvent.h"
#include "Core/Components/PBInventoryComponent.h"
#include "Core/Components/PBSanityComponent.h"
#include "Core/Character/PBCharacterBase.h"
#include "PensionBenedito.h"

// Base event
UPBDialogueEvent::UPBDialogueEvent()
{
}

void UPBDialogueEvent::ExecuteEvent_Implementation(AActor* Context)
{
    // Default implementation - override in subclasses
}

FText UPBDialogueEvent::GetDescription() const
{
    return FText::FromString("Base Event");
}

// Give Item event
UPBEvent_GiveItem::UPBEvent_GiveItem()
{
    ItemID = NAME_None;
    Quantity = 1;
}

void UPBEvent_GiveItem::ExecuteEvent_Implementation(AActor* Context)
{
    APBCharacterBase* Character = Cast<APBCharacterBase>(Context);
    if (!Character || !Character->GetInventoryComponent())
    {
        UE_LOG(LogPBDialogue, Warning, TEXT("Cannot give item - invalid character or inventory"));
        return;
    }

    // TODO: Find item definition by ID and add to inventory
    UE_LOG(LogPBDialogue, Log, TEXT("Giving %d x %s to player"), Quantity, *ItemID.ToString());
}

FText UPBEvent_GiveItem::GetDescription() const
{
    return FText::Format(
        FText::FromString("Give {0} x {1}"),
        FText::AsNumber(Quantity),
        FText::FromName(ItemID)
    );
}

// Set Flag event
UPBEvent_SetFlag::UPBEvent_SetFlag()
{
    FlagName = NAME_None;
    FlagValue = true;
}

void UPBEvent_SetFlag::ExecuteEvent_Implementation(AActor* Context)
{
    // TODO: Set flag in narrative manager
    UE_LOG(LogPBNarrative, Log, TEXT("Setting flag '%s' to %s"),
        *FlagName.ToString(), FlagValue ? TEXT("true") : TEXT("false"));
}

FText UPBEvent_SetFlag::GetDescription() const
{
    return FText::Format(
        FText::FromString("Set flag '{0}' to {1}"),
        FText::FromName(FlagName),
        FlagValue ? FText::FromString("true") : FText::FromString("false")
    );
}

// Modify Sanity event
UPBEvent_ModifySanity::UPBEvent_ModifySanity()
{
    SanityChange = -10.0f;
    bIsHorrorEvent = false;
}

void UPBEvent_ModifySanity::ExecuteEvent_Implementation(AActor* Context)
{
    APBCharacterBase* Character = Cast<APBCharacterBase>(Context);
    if (!Character || !Character->GetSanityComponent())
    {
        UE_LOG(LogPBTerror, Warning, TEXT("Cannot modify sanity - invalid character or component"));
        return;
    }

    if (bIsHorrorEvent)
    {
        Character->GetSanityComponent()->ApplyHorrorEvent(FMath::Abs(SanityChange) / 10.0f);
    }
    else
    {
        Character->GetSanityComponent()->ModifySanity(SanityChange);
    }

    UE_LOG(LogPBTerror, Log, TEXT("Sanity modified by %.1f (horror: %s)"),
        SanityChange, bIsHorrorEvent ? TEXT("true") : TEXT("false"));
}

FText UPBEvent_ModifySanity::GetDescription() const
{
    return FText::Format(
        FText::FromString("Modify sanity by {0} (horror: {1})"),
        FText::AsNumber(SanityChange),
        bIsHorrorEvent ? FText::FromString("true") : FText::FromString("false")
    );
}