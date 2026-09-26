// Copyright Pension Benedito, 2024. All rights reserved.

#include "Dialogue/PBDialogueCondition.h"
#include "Core/Components/PBInventoryComponent.h"
#include "Core/Character/PBCharacterBase.h"
#include "PensionBenedito.h"

// Base condition
UPBDialogueCondition::UPBDialogueCondition()
{
}

bool UPBDialogueCondition::CheckCondition_Implementation(AActor* Context) const
{
    // Default implementation - override in subclasses
    return true;
}

FText UPBDialogueCondition::GetDescription() const
{
    return FText::FromString("Base Condition");
}

// Has Item condition
UPBCondition_HasItem::UPBCondition_HasItem()
{
    ItemID = NAME_None;
    RequiredQuantity = 1;
}

bool UPBCondition_HasItem::CheckCondition_Implementation(AActor* Context) const
{
    APBCharacterBase* Character = Cast<APBCharacterBase>(Context);
    if (!Character || !Character->GetInventoryComponent())
    {
        return false;
    }

    // TODO: Find item by ID in inventory
    // For now, return true
    return true;
}

FText UPBCondition_HasItem::GetDescription() const
{
    return FText::Format(
        FText::FromString("Has {0} x {1}"),
        FText::AsNumber(RequiredQuantity),
        FText::FromName(ItemID)
    );
}

// Flag Set condition
UPBCondition_FlagSet::UPBCondition_FlagSet()
{
    FlagName = NAME_None;
    bExpectedValue = true;
}

bool UPBCondition_FlagSet::CheckCondition_Implementation(AActor* Context) const
{
    // TODO: Check narrative flag from narrative manager
    // For now, return expected value
    return bExpectedValue;
}

FText UPBCondition_FlagSet::GetDescription() const
{
    return FText::Format(
        FText::FromString("Flag '{0}' is {1}"),
        FText::FromName(FlagName),
        bExpectedValue ? FText::FromString("true") : FText::FromString("false")
    );
}