// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBInventoryComponent.generated.h"

class UPBItemDefinition;

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPBItemDefinition* ItemDefinition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotIndex;

    FInventoryItem()
        : ItemDefinition(nullptr)
        , Quantity(0)
        , SlotIndex(-1)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, const FInventoryItem&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, const FInventoryItem&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

/**
 * Inventory component for managing items.
 * Supports stacking, slot-based storage, and item queries.
 * Used for documents, keys, tools, and other collectibles.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PENSIONBENEDITO_API UPBInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPBInventoryComponent();

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemAdded OnItemAdded;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemRemoved OnItemRemoved;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

    // Inventory settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlots;

    // Add item to inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UPBItemDefinition* ItemDefinition, int32 Quantity = 1);

    // Remove item from inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(UPBItemDefinition* ItemDefinition, int32 Quantity = 1);

    // Remove item at specific slot
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemAtSlot(int32 SlotIndex);

    // Check if player has item
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(UPBItemDefinition* ItemDefinition, int32 Quantity = 1) const;

    // Get item quantity
    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemQuantity(UPBItemDefinition* ItemDefinition) const;

    // Get all items
    UFUNCTION(BlueprintPure, Category = "Inventory")
    TArray<FInventoryItem> GetAllItems() const { return Items; }

    // Get item at slot
    UFUNCTION(BlueprintPure, Category = "Inventory")
    FInventoryItem GetItemAtSlot(int32 SlotIndex) const;

    // Find item by definition
    UFUNCTION(BlueprintPure, Category = "Inventory")
    FInventoryItem FindItem(UPBItemDefinition* ItemDefinition) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventoryItem> Items;

    // Find existing stack for item
    int32 FindExistingStack(UPBItemDefinition* ItemDefinition) const;

    // Find first empty slot
    int32 FindEmptySlot() const;
};