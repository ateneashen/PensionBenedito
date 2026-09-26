// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBDialogueCondition.generated.h"

/**
 * Base class for dialogue conditions.
 * Conditions determine if a dialogue node or choice should be available.
 * Subclass to create custom conditions (has item, flag set, etc.)
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBDialogueCondition : public UObject
{
    GENERATED_BODY()

public:
    UPBDialogueCondition();

    // Check if condition is met
    UFUNCTION(BlueprintNativeEvent, Category = "Condition")
    bool CheckCondition(AActor* Context) const;
    virtual bool CheckCondition_Implementation(AActor* Context) const;

    // Get condition description for debugging
    UFUNCTION(BlueprintPure, Category = "Condition")
    virtual FText GetDescription() const;

protected:
    // Override to implement custom condition logic
    UFUNCTION(BlueprintImplementableEvent, Category = "Condition")
    bool OnCheckCondition(AActor* Context) const;
};

/**
 * Condition: Check if player has a specific item
 */
UCLASS(BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBCondition_HasItem : public UPBDialogueCondition
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
    int32 RequiredQuantity;

    UPBCondition_HasItem();

    virtual bool CheckCondition_Implementation(AActor* Context) const override;
    virtual FText GetDescription() const override;
};

/**
 * Condition: Check if a narrative flag is set
 */
UCLASS(BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBCondition_FlagSet : public UPBDialogueCondition
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
    FName FlagName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
    bool bExpectedValue;

    UPBCondition_FlagSet();

    virtual bool CheckCondition_Implementation(AActor* Context) const override;
    virtual FText GetDescription() const override;
};