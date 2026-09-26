// Copyright Pension Benedito, 2024. All rights reserved.

#include "Interaction/PBItemDefinition.h"

UPBItemDefinition::UPBItemDefinition()
{
    ItemID = NAME_None;
    DisplayName = FText::FromString("Item");
    Description = FText::FromString("An item");
    ItemType = EItemType::Misc;
    Icon = nullptr;
    WorldMesh = nullptr;
    bIsStackable = true;
    MaxStackSize = 99;
    bIsConsumable = false;
    bIsKeyItem = false;
    DocumentContent = FText::GetEmpty();
    DocumentImage = nullptr;
    OnUseEffect = nullptr;
    NarrativeFlag = NAME_None;
}

FPrimaryAssetId UPBItemDefinition::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Item", ItemID);
}