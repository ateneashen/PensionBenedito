// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NAKInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNAKOnItemAdded, FName, ItemID, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNAKOnItemRemoved, FName, ItemID, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNAKOnInventoryChanged);

/**
 * Represents a single item stack in the inventory.
 * Items are identified by FName IDs, not asset references, making this
 * work with ANY game. Games can extend this by adding their own item lookup systems.
 */
USTRUCT(BlueprintType)
struct NARRATIVEACTIONKIT_API FNAKInventoryItem
{
	GENERATED_BODY()

	/** Unique identifier for the item type (e.g. "Sword_Iron", "Potion_Health") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	FName ItemID;

	/** Localized display name for UI purposes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	FText DisplayName;

	/** Current stack quantity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	int32 Quantity;

	/** Slot index this item occupies in the inventory grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	int32 SlotIndex;

	/** Category/type tag for filtering (e.g. "Weapon", "Consumable", "Material") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	FName ItemType;

	/** If true, this item cannot be dropped or sold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory")
	bool bIsKeyItem;

	FNAKInventoryItem()
		: ItemID(NAME_None)
		, DisplayName(FText::GetEmpty())
		, Quantity(0)
		, SlotIndex(INDEX_NONE)
		, ItemType(NAME_None)
		, bIsKeyItem(false)
	{}
};

/**
 * Reusable inventory component for NarrativeActionKit.
 * 
 * Manages a slot-based inventory with item stacking. Items are tracked
 * by FName IDs, making this component game-agnostic. Games extend this
 * by pairing it with their own item data tables or lookup systems.
 *
 * Features:
 * - Configurable slot count
 * - Item stacking by ItemID
 * - Key item protection
 * - Blueprint-friendly delegates for UI binding
 * - Slot-based management for grid UIs
 *
 * Usage:
 *   Add to your character or pawn. Bind to OnItemAdded / OnItemRemoved
 *   for UI updates. Use AddItem/RemoveItem for gameplay logic.
 */
UCLASS(ClassGroup = (NarrativeActionKit), meta = (BlueprintSpawnableComponent))
class NARRATIVEACTIONKIT_API UNAKInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNAKInventoryComponent();

	// ------------------------------------------------------------------
	// Configuration
	// ------------------------------------------------------------------

	/** Maximum number of inventory slots. Items cannot be added when all slots are full. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NAK|Inventory", meta = (ClampMin = "1"))
	int32 MaxSlots;

	// ------------------------------------------------------------------
	// Delegates
	// ------------------------------------------------------------------

	/** Broadcast when an item is added. Passes ItemID and quantity added. */
	UPROPERTY(BlueprintAssignable, Category = "NAK|Inventory")
	FNAKOnItemAdded OnItemAdded;

	/** Broadcast when an item is removed. Passes ItemID and quantity removed. */
	UPROPERTY(BlueprintAssignable, Category = "NAK|Inventory")
	FNAKOnItemRemoved OnItemRemoved;

	/** Broadcast whenever the inventory contents change (add, remove, or rearrange). */
	UPROPERTY(BlueprintAssignable, Category = "NAK|Inventory")
	FNAKOnInventoryChanged OnInventoryChanged;

	// ------------------------------------------------------------------
	// Core Operations
	// ------------------------------------------------------------------

	/**
	 * Add an item to the inventory. Stacks with existing items of the same ID.
	 * @param ItemID      Unique item identifier.
	 * @param DisplayName Localized name for UI display.
	 * @param Quantity    Number of items to add (must be > 0).
	 * @param ItemType    Category tag for filtering.
	 * @param bIsKeyItem  If true, the item cannot be dropped or sold.
	 * @return True if the item was fully added. False if inventory is full or Quantity <= 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "NAK|Inventory")
	bool AddItem(FName ItemID, FText DisplayName, int32 Quantity, FName ItemType = NAME_None, bool bIsKeyItem = false);

	/**
	 * Remove a quantity of an item from the inventory.
	 * @param ItemID   Item identifier to remove.
	 * @param Quantity Number of items to remove (must be > 0).
	 * @return True if the requested quantity was removed. False if insufficient quantity or item not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "NAK|Inventory")
	bool RemoveItem(FName ItemID, int32 Quantity);

	/**
	 * Check whether the inventory contains at least the given quantity of an item.
	 * @param ItemID   Item identifier to check.
	 * @param Quantity Minimum quantity required.
	 * @return True if the inventory has enough of the item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	bool HasItem(FName ItemID, int32 Quantity = 1) const;

	/**
	 * Get the total quantity of a specific item across all stacks.
	 * @param ItemID Item identifier to query.
	 * @return Total quantity held, or 0 if not found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	int32 GetItemQuantity(FName ItemID) const;

	/**
	 * Get a copy of all items currently in the inventory.
	 * @return Array of all inventory items.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	TArray<FNAKInventoryItem> GetAllItems() const;

	/**
	 * Get the item at a specific slot index.
	 * @param SlotIndex Zero-based slot index.
	 * @param OutItem   Receives the item data if the slot is occupied.
	 * @return True if the slot contains an item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	bool GetItemAtSlot(int32 SlotIndex, FNAKInventoryItem& OutItem) const;

	/**
	 * Find a specific item by ID. Returns the first matching stack.
	 * @param ItemID Item identifier to find.
	 * @param OutItem Receives the item data if found.
	 * @return True if the item exists in the inventory.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	bool FindItem(FName ItemID, FNAKInventoryItem& OutItem) const;

	/**
	 * Get the number of currently occupied slots.
	 * @return Count of slots that contain items.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	int32 GetUsedSlotCount() const;

	/**
	 * Check whether the inventory has any free slots.
	 * @return True if at least one slot is empty.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NAK|Inventory")
	bool HasFreeSlot() const;

protected:
	/** The actual inventory storage. Index = slot position. */
	UPROPERTY(VisibleAnywhere, Category = "NAK|Inventory")
	TArray<FNAKInventoryItem> Items;

private:
	/** Find the slot index of an existing stack for the given ItemID, or INDEX_NONE. */
	int32 FindStackIndex(FName ItemID) const;

	/** Find the first empty slot index, or INDEX_NONE if inventory is full. */
	int32 FindEmptySlotIndex() const;
};