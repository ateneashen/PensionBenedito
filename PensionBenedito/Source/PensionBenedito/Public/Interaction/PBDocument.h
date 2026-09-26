// Copyright Pension Benedito, 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/PBInteractableBase.h"
#include "PBDocument.generated.h"

class UPBItemDefinition;

/**
 * Document interactable for Pension Benedito.
 * Represents letters, notes, newspapers, and other readable items.
 * Integrates with inventory and narrative systems.
 * Period setting: 1930s Spain documents with appropriate styling.
 */
UCLASS()
class PENSIONBENEDITO_API APBDocument : public APBInteractableBase
{
    GENERATED_BODY()

public:
    APBDocument();

    // Document properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Document")
    UPBItemDefinition* DocumentItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Document")
    bool bAutoPickup;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Document")
    bool bAddToInventory;

    // Get document content
    UFUNCTION(BlueprintPure, Category = "Document")
    FText GetDocumentContent() const;

    // Get document title
    UFUNCTION(BlueprintPure, Category = "Document")
    FText GetDocumentTitle() const;

protected:
    virtual void OnInteraction(AActor* Interactor) override;
    virtual bool CheckInteractionConditions(AActor* Interactor) const override;

private:
    UPROPERTY()
    bool bHasBeenRead;
};