// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Components/NAKInventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogNAKInventory, Log, All);

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

UNAKInventoryComponent::UNAKInventoryComponent()
	: MaxSlots(20)
{
	PrimaryComponentTick.bCanEverTick = false;
}

// ---------------------------------------------------------------------------
// Core Operations
// ---------------------------------------------------------------------------

bool UNAKInventoryComponent::AddItem(FName ItemID, FText DisplayName, int32 Quantity, FName ItemType, bool bIsKeyItem)
{
	if (ItemID == NAME_None || Quantity <= 0)
	{
		UE_LOG(LogNAKInventory, Warning, TEXT("AddItem: Invalid parameters - ItemID: %s, Quantity: %d"), *ItemID.ToString(), Quantity);
		return false;
	}

	// Try to stack with an existing item of the same ID
	const int32 ExistingIndex = FindStackIndex(ItemID);
	if (ExistingIndex != INDEX_NONE)
	{
		Items[ExistingIndex].Quantity += Quantity;
		OnItemAdded.Broadcast(ItemID, Quantity);
		OnInventoryChanged.Broadcast();
		UE_LOG(LogNAKInventory, Verbose, TEXT("AddItem: Stacked %d x %s (total: %d) in slot %d"),
			Quantity, *ItemID.ToString(), Items[ExistingIndex].Quantity, ExistingIndex);
		return true;
	}

	// No existing stack — find an empty slot
	const int32 EmptyIndex = FindEmptySlotIndex();
	if (EmptyIndex == INDEX_NONE)
	{
		UE_LOG(LogNAKInventory, Warning, TEXT("AddItem: Inventory full, cannot add %s"), *ItemID.ToString());
		return false;
	}

	// Populate the empty slot
	FNAKInventoryItem& NewItem = Items[EmptyIndex];
	NewItem.ItemID = ItemID;
	NewItem.DisplayName = DisplayName;
	NewItem.Quantity = Quantity;
	NewItem.SlotIndex = EmptyIndex;
	NewItem.ItemType = ItemType;
	NewItem.bIsKeyItem = bIsKeyItem;

	OnItemAdded.Broadcast(ItemID, Quantity);
	OnInventoryChanged.Broadcast();

	UE_LOG(LogNAKInventory, Verbose, TEXT("AddItem: Added %d x %s to slot %d"),
		Quantity, *ItemID.ToString(), EmptyIndex);
	return true;
}

bool UNAKInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
	if (ItemID == NAME_None || Quantity <= 0)
	{
		UE_LOG(LogNAKInventory, Warning, TEXT("RemoveItem: Invalid parameters - ItemID: %s, Quantity: %d"), *ItemID.ToString(), Quantity);
		return false;
	}

	const int32 StackIndex = FindStackIndex(ItemID);
	if (StackIndex == INDEX_NONE)
	{
		UE_LOG(LogNAKInventory, Warning, TEXT("RemoveItem: Item %s not found in inventory"), *ItemID.ToString());
		return false;
	}

	FNAKInventoryItem& Item = Items[StackIndex];
	if (Item.Quantity < Quantity)
	{
		UE_LOG(LogNAKInventory, Warning, TEXT("RemoveItem: Insufficient quantity for %s (have %d, need %d)"),
			*ItemID.ToString(), Item.Quantity, Quantity);
		return false;
	}

	Item.Quantity -= Quantity;

	if (Item.Quantity <= 0)
	{
		// Clear the slot entirely
		Items[StackIndex] = FNAKInventoryItem();
		UE_LOG(LogNAKInventory, Verbose, TEXT("RemoveItem: Removed all %s from slot %d"), *ItemID.ToString(), StackIndex);
	}
	else
	{
		UE_LOG(LogNAKInventory, Verbose, TEXT("RemoveItem: Removed %d x %s (remaining: %d) from slot %d"),
			Quantity, *ItemID.ToString(), Item.Quantity, StackIndex);
	}

	OnItemRemoved.Broadcast(ItemID, Quantity);
	OnInventoryChanged.Broadcast();
	return true;
}

bool UNAKInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
	return GetItemQuantity(ItemID) >= Quantity;
}

int32 UNAKInventoryComponent::GetItemQuantity(FName ItemID) const
{
	const int32 StackIndex = FindStackIndex(ItemID);
	if (StackIndex == INDEX_NONE)
	{
		return 0;
	}
	return Items[StackIndex].Quantity;
}

TArray<FNAKInventoryItem> UNAKInventoryComponent::GetAllItems() const
{
	TArray<FNAKInventoryItem> Result;
	Result.Reserve(Items.Num());

	for (const FNAKInventoryItem& Item : Items)
	{
		if (Item.ItemID != NAME_None)
		{
			Result.Add(Item);
		}
	}
	return Result;
}

bool UNAKInventoryComponent::GetItemAtSlot(int32 SlotIndex, FNAKInventoryItem& OutItem) const
{
	if (!Items.IsValidIndex(SlotIndex))
	{
		return false;
	}

	const FNAKInventoryItem& Item = Items[SlotIndex];
	if (Item.ItemID == NAME_None)
	{
		return false;
	}

	OutItem = Item;
	return true;
}

bool UNAKInventoryComponent::FindItem(FName ItemID, FNAKInventoryItem& OutItem) const
{
	const int32 StackIndex = FindStackIndex(ItemID);
	if (StackIndex == INDEX_NONE)
	{
		return false;
	}

	OutItem = Items[StackIndex];
	return true;
}

int32 UNAKInventoryComponent::GetUsedSlotCount() const
{
	int32 Count = 0;
	for (const FNAKInventoryItem& Item : Items)
	{
		if (Item.ItemID != NAME_None)
		{
			++Count;
		}
	}
	return Count;
}

bool UNAKInventoryComponent::HasFreeSlot() const
{
	return GetUsedSlotCount() < MaxSlots;
}

// ---------------------------------------------------------------------------
// Internal Helpers
// ---------------------------------------------------------------------------

int32 UNAKInventoryComponent::FindStackIndex(FName ItemID) const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UNAKInventoryComponent::FindEmptySlotIndex() const
{
	// First pass: look for an existing empty slot in the array
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == NAME_None)
		{
			return i;
		}
	}

	// Second pass: grow the array if we haven't hit MaxSlots yet
	if (Items.Num() < MaxSlots)
	{
		const int32 NewIndex = Items.Num();
		Items.Add(FNAKInventoryItem());
		return NewIndex;
	}

	return INDEX_NONE;
}