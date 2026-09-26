// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Document    UMETA(DisplayName = "Document"),
    Key         UMETA(DisplayName = "Key Item"),
    Tool        UMETA(DisplayName = "Tool"),
    Consumable  UMETA(DisplayName = "Consumable"),
    Quest       UMETA(DisplayName = "Quest Item"),
    Misc        UMETA(DisplayName = "Miscellaneous")
};

/**
 * Data asset defining an item's properties.
 * Used by the inventory system and interaction system.
 * Supports different item types: documents, keys, tools, etc.
 */
UCLASS(BlueprintType)
class PENSIONBENEDITO_API UPBItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Item identification
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FName ItemID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    EItemType ItemType;

    // Visual
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    UTexture2D* Icon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    UStaticMesh* WorldMesh;

    // Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    bool bIsStackable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    int32 MaxStackSize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    bool bIsConsumable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    bool bIsKeyItem;

    // Document-specific
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Document", meta = (EditCondition = "ItemType == EItemType::Document"))
    FText DocumentContent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Document", meta = (EditCondition = "ItemType == EItemType::Document"))
    UTexture2D* DocumentImage;

    // Gameplay effects
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
    TSubclassOf<UGameplayEffect> OnUseEffect;

    // Narrative flags
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Narrative")
    FName NarrativeFlag;

    // Constructor
    UPBItemDefinition();

    // Get the asset type
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};