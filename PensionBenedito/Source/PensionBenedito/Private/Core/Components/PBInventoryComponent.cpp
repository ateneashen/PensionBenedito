// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Components/PBInventoryComponent.h"
#include "Interaction/PBItemDefinition.h"
#include "PensionBenedito.h"

UPBInventoryComponent::UPBInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxSlots = 20;
}

bool UPBInventoryComponent::AddItem(UPBItemDefinition* ItemDefinition, int32 Quantity)
{
    if (!ItemDefinition || Quantity <= 0)
    {
        return false;
    }

    // Try to stack with existing item
    int32 ExistingIndex = FindExistingStack(ItemDefinition);
    if (ExistingIndex != INDEX_NONE)
    {
        Items[ExistingIndex].Quantity += Quantity;
        OnItemAdded.Broadcast(Items[ExistingIndex]);
        OnInventoryChanged.Broadcast();
        UE_LOG(LogPBInteraction, Log, TEXT("Added %d x %s to existing stack (total: %d)"),
            Quantity, *ItemDefinition->GetName(), Items[ExistingIndex].Quantity);
        return true;
    }

    // Find empty slot for new stack
    int32 EmptySlot = FindEmptySlot();
    if (EmptySlot == INDEX_NONE)
    {
        UE_LOG(LogPBInteraction, Warning, TEXT("Inventory full, cannot add %s"), *ItemDefinition->GetName());
        return false;
    }

    // Add new item
    FInventoryItem NewItem;
    NewItem.ItemDefinition = ItemDefinition;
    NewItem.Quantity = Quantity;
    NewItem.SlotIndex = EmptySlot;
    Items.Add(NewItem);

    OnItemAdded.Broadcast(NewItem);
    OnInventoryChanged.Broadcast();

    UE_LOG(LogPBInteraction, Log, TEXT("Added %d x %s to slot %d"),
        Quantity, *ItemDefinition->GetName(), EmptySlot);

    return true;
}

bool UPBInventoryComponent::RemoveItem(UPBItemDefinition* ItemDefinition, int32 Quantity)
{
    if (!ItemDefinition || Quantity <= 0)
    {
        return false;
    }

    for (int32 i = Items.Num() - 1; i >= 0; --i)
    {
        if (Items[i].ItemDefinition == ItemDefinition)
        {
            if (Items[i].Quantity >= Quantity)
            {
                Items[i].Quantity -= Quantity;

                if (Items[i].Quantity <= 0)
                {
                    FInventoryItem RemovedItem = Items[i];
                    Items.RemoveAt(i);
                    OnItemRemoved.Broadcast(RemovedItem);
                }

                OnInventoryChanged.Broadcast();
                UE_LOG(LogPBInteraction, Log, TEXT("Removed %d x %s"), Quantity, *ItemDefinition->GetName());
                return true;
            }
        }
    }

    UE_LOG(LogPBInteraction, Warning, TEXT("Cannot remove %d x %s - not enough in inventory"),
        Quantity, *ItemDefinition->GetName());
    return false;
}

bool UPBInventoryComponent::RemoveItemAtSlot(int32 SlotIndex)
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].SlotIndex == SlotIndex)
        {
            FInventoryItem RemovedItem = Items[i];
            Items.RemoveAt(i);
            OnItemRemoved.Broadcast(RemovedItem);
            OnInventoryChanged.Broadcast();
            return true;
        }
    }

    return false;
}

bool UPBInventoryComponent::HasItem(UPBItemDefinition* ItemDefinition, int32 Quantity) const
{
    return GetItemQuantity(ItemDefinition) >= Quantity;
}

int32 UPBInventoryComponent::GetItemQuantity(UPBItemDefinition* ItemDefinition) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.ItemDefinition == ItemDefinition)
        {
            return Item.Quantity;
        }
    }

    return 0;
}

FInventoryItem UPBInventoryComponent::GetItemAtSlot(int32 SlotIndex) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.SlotIndex == SlotIndex)
        {
            return Item;
        }
    }

    return FInventoryItem();
}

FInventoryItem UPBInventoryComponent::FindItem(UPBItemDefinition* ItemDefinition) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.ItemDefinition == ItemDefinition)
        {
            return Item;
        }
    }

    return FInventoryItem();
}

int32 UPBInventoryComponent::FindExistingStack(UPBItemDefinition* ItemDefinition) const
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].ItemDefinition == ItemDefinition)
        {
            return i;
        }
    }

    return INDEX_NONE;
}

int32 UPBInventoryComponent::FindEmptySlot() const
{
    for (int32 Slot = 0; Slot < MaxSlots; ++Slot)
    {
        bool bSlotUsed = false;
        for (const FInventoryItem& Item : Items)
        {
            if (Item.SlotIndex == Slot)
            {
                bSlotUsed = true;
                break;
            }
        }

        if (!bSlotUsed)
        {
            return Slot;
        }
    }

    return INDEX_NONE;
}