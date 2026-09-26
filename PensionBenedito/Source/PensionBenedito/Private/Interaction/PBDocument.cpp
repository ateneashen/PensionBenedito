// Copyright Pension Benedito, 2024. All rights reserved.

#include "Interaction/PBDocument.h"
#include "Interaction/PBItemDefinition.h"
#include "Core/Components/PBInventoryComponent.h"
#include "Core/Character/PBCharacterBase.h"
#include "PensionBenedito.h"

APBDocument::APBDocument()
{
    bAutoPickup = false;
    bAddToInventory = true;
    bHasBeenRead = false;

    // Set default prompt
    InteractionPromptText = FText::FromString("Read document");
    ObjectName = FText::FromString("Document");
}

FText APBDocument::GetDocumentContent() const
{
    if (DocumentItem)
    {
        return DocumentItem->DocumentContent;
    }

    return FText::GetEmpty();
}

FText APBDocument::GetDocumentTitle() const
{
    if (DocumentItem)
    {
        return DocumentItem->DisplayName;
    }

    return FText::GetEmpty();
}

void APBDocument::OnInteraction(AActor* Interactor)
{
    // Call parent implementation
    Super::OnInteraction(Interactor);

    if (!DocumentItem)
    {
        UE_LOG(LogPBInteraction, Warning, TEXT("Document %s has no item definition"), *GetName());
        return;
    }

    // Mark as read
    bHasBeenRead = true;

    // Add to inventory if configured
    if (bAddToInventory)
    {
        APBCharacterBase* Character = Cast<APBCharacterBase>(Interactor);
        if (Character && Character->GetInventoryComponent())
        {
            Character->GetInventoryComponent()->AddItem(DocumentItem, 1);
            UE_LOG(LogPBInteraction, Log, TEXT("Added document to inventory: %s"), *DocumentItem->DisplayName.ToString());
        }
    }

    // Set narrative flag
    if (DocumentItem->NarrativeFlag != NAME_None)
    {
        // TODO: Set narrative flag in narrative manager
        UE_LOG(LogPBNarrative, Log, TEXT("Narrative flag set: %s"), *DocumentItem->NarrativeFlag.ToString());
    }

    // TODO: Show document UI
    UE_LOG(LogPBInteraction, Log, TEXT("Document read: %s"), *GetDocumentTitle().ToString());
}

bool APBDocument::CheckInteractionConditions(AActor* Interactor) const
{
    // Can always read documents
    return true;
}