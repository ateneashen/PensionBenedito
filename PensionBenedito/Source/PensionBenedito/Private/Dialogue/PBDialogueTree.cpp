// Copyright Pension Benedito, 2024. All rights reserved.

#include "Dialogue/PBDialogueTree.h"
#include "Dialogue/PBDialogueNode.h"
#include "PensionBenedito.h"

UPBDialogueTree::UPBDialogueTree()
{
    DialogueID = NAME_None;
    DialogueName = FText::FromString("Dialogue");
    Description = FText::FromString("A dialogue tree");
    StartingNode = nullptr;
}

UPBDialogueNode* UPBDialogueTree::FindNodeBySpeaker(FName SpeakerID) const
{
    for (UPBDialogueNode* Node : Nodes)
    {
        if (Node && Node->SpeakerID == SpeakerID)
        {
            return Node;
        }
    }

    return nullptr;
}

FPrimaryAssetId UPBDialogueTree::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Dialogue", DialogueID);
}

bool UPBDialogueTree::ValidateDialogueTree() const
{
    bool bIsValid = true;

    // Check starting node
    if (!StartingNode)
    {
        UE_LOG(LogPBDialogue, Error, TEXT("Dialogue tree %s has no starting node"), *DialogueName.ToString());
        bIsValid = false;
    }

    // Check all nodes
    for (int32 i = 0; i < Nodes.Num(); ++i)
    {
        UPBDialogueNode* Node = Nodes[i];
        if (!Node)
        {
            UE_LOG(LogPBDialogue, Error, TEXT("Dialogue tree %s has null node at index %d"), *DialogueName.ToString(), i);
            bIsValid = false;
            continue;
        }

        // Check end nodes
        if (Node->bIsEndNode)
        {
            if (Node->NextNode)
            {
                UE_LOG(LogPBDialogue, Warning, TEXT("End node %s has a next node"), *Node->GetName());
            }
        }
        else
        {
            // Non-end nodes should have next node or choices
            if (!Node->NextNode && Node->Choices.Num() == 0)
            {
                UE_LOG(LogPBDialogue, Warning, TEXT("Node %s has no next node and no choices"), *Node->GetName());
            }
        }

        // Check choices
        for (const FDialogueChoice& Choice : Node->Choices)
        {
            if (!Choice.NextNode)
            {
                UE_LOG(LogPBDialogue, Warning, TEXT("Choice '%s' in node %s has no next node"),
                    *Choice.ChoiceText.ToString(), *Node->GetName());
            }
        }
    }

    if (bIsValid)
    {
        UE_LOG(LogPBDialogue, Log, TEXT("Dialogue tree %s validated successfully"), *DialogueName.ToString());
    }

    return bIsValid;
}