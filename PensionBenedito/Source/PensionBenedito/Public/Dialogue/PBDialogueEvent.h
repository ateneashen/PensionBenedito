// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBDialogueEvent.generated.h"

/**
 * Base class for dialogue events.
 * Events execute when a dialogue node is reached.
 * Subclass to create custom events (give item, set flag, play animation, etc.)
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBDialogueEvent : public UObject
{
    GENERATED_BODY()

public:
    UPBDialogueEvent();

    // Execute the event
    UFUNCTION(BlueprintNativeEvent, Category = "Event")
    void ExecuteEvent(AActor* Context);
    virtual void ExecuteEvent_Implementation(AActor* Context);

    // Get event description for debugging
    UFUNCTION(BlueprintPure, Category = "Event")
    virtual FText GetDescription() const;

protected:
    // Override to implement custom event logic
    UFUNCTION(BlueprintImplementableEvent, Category = "Event")
    void OnExecuteEvent(AActor* Context);
};

/**
 * Event: Give item to player
 */
UCLASS(BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBEvent_GiveItem : public UPBDialogueEvent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    int32 Quantity;

    UPBEvent_GiveItem();

    virtual void ExecuteEvent_Implementation(AActor* Context) override;
    virtual FText GetDescription() const override;
};

/**
 * Event: Set narrative flag
 */
UCLASS(BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBEvent_SetFlag : public UPBDialogueEvent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    FName FlagName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    bool FlagValue;

    UPBEvent_SetFlag();

    virtual void ExecuteEvent_Implementation(AActor* Context) override;
    virtual FText GetDescription() const override;
};

/**
 * Event: Modify sanity
 */
UCLASS(BlueprintType, EditInlineNew)
class PENSIONBENEDITO_API UPBEvent_ModifySanity : public UPBDialogueEvent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    float SanityChange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
    bool bIsHorrorEvent;

    UPBEvent_ModifySanity();

    virtual void ExecuteEvent_Implementation(AActor* Context) override;
    virtual FText GetDescription() const override;
};