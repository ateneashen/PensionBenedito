// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Components/PBDialogueComponent.h"
#include "Dialogue/PBDialogueTree.h"
#include "Dialogue/PBDialogueNode.h"
#include "PensionBenedito.h"

UPBDialogueComponent::UPBDialogueComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentDialogueTree = nullptr;
    CurrentNode = nullptr;
    CurrentNodeIndex = -1;
    bIsDialogueActive = false;
}

void UPBDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UPBDialogueComponent::StartDialogue(UPBDialogueTree* DialogueTree)
{
    if (!DialogueTree || bIsDialogueActive)
    {
        UE_LOG(LogPBDialogue, Warning, TEXT("Cannot start dialogue - invalid tree or already in dialogue"));
        return false;
    }

    // Get the first node
    TArray<UPBDialogueNode*> Nodes = DialogueTree->GetNodes();
    if (Nodes.Num() == 0)
    {
        UE_LOG(LogPBDialogue, Error, TEXT("Dialogue tree has no nodes"));
        return false;
    }

    // Initialize dialogue
    CurrentDialogueTree = DialogueTree;
    CurrentNodeIndex = 0;
    CurrentNode = Nodes[0];
    bIsDialogueActive = true;

    // Check conditions and process events
    if (CheckNodeConditions(CurrentNode))
    {
        ProcessNodeEvents(CurrentNode);
        OnDialogueStarted.Broadcast(DialogueTree);
        OnDialogueNodeChanged.Broadcast(CurrentNode, CurrentNodeIndex);

        UE_LOG(LogPBDialogue, Log, TEXT("Dialogue started: %s"), *DialogueTree->GetName());
        return true;
    }
    else
    {
        // First node failed conditions
        EndDialogue();
        return false;
    }
}

void UPBDialogueComponent::EndDialogue()
{
    if (!bIsDialogueActive)
    {
        return;
    }

    UPBDialogueTree* EndedTree = CurrentDialogueTree;

    // Reset state
    CurrentDialogueTree = nullptr;
    CurrentNode = nullptr;
    CurrentNodeIndex = -1;
    bIsDialogueActive = false;

    OnDialogueEnded.Broadcast(EndedTree);

    UE_LOG(LogPBDialogue, Log, TEXT("Dialogue ended"));
}

void UPBDialogueComponent::ProgressDialogue()
{
    if (!bIsDialogueActive || !CurrentNode)
    {
        return;
    }

    // Check if node has choices (wait for player input)
    if (CurrentNode->GetChoices().Num() > 0)
    {
        UE_LOG(LogPBDialogue, Warning, TEXT("Cannot progress - node has choices"));
        return;
    }

    // Get next node
    UPBDialogueNode* NextNode = CurrentNode->GetNextNode();
    if (NextNode)
    {
        CurrentNode = NextNode;
        CurrentNodeIndex++;

        if (CheckNodeConditions(CurrentNode))
        {
            ProcessNodeEvents(CurrentNode);
            OnDialogueNodeChanged.Broadcast(CurrentNode, CurrentNodeIndex);
        }
        else
        {
            // Conditions failed, try next
            ProgressDialogue();
        }
    }
    else
    {
        // No next node - end dialogue
        EndDialogue();
    }
}

void UPBDialogueComponent::MakeChoice(int32 ChoiceIndex)
{
    if (!bIsDialogueActive || !CurrentNode)
    {
        return;
    }

    TArray<FDialogueChoice> Choices = CurrentNode->GetChoices();
    if (ChoiceIndex < 0 || ChoiceIndex >= Choices.Num())
    {
        UE_LOG(LogPBDialogue, Error, TEXT("Invalid choice index: %d"), ChoiceIndex);
        return;
    }

    // Broadcast choice
    OnDialogueChoiceMade.Broadcast(ChoiceIndex);

    // Get next node from choice
    UPBDialogueNode* NextNode = Choices[ChoiceIndex].NextNode;
    if (NextNode)
    {
        CurrentNode = NextNode;
        CurrentNodeIndex++;

        if (CheckNodeConditions(CurrentNode))
        {
            ProcessNodeEvents(CurrentNode);
            OnDialogueNodeChanged.Broadcast(CurrentNode, CurrentNodeIndex);
        }
        else
        {
            // Conditions failed, try next
            ProgressDialogue();
        }
    }
    else
    {
        // No next node - end dialogue
        EndDialogue();
    }
}

FText UPBDialogueComponent::GetCurrentSpeakerName() const
{
    if (CurrentNode)
    {
        return CurrentNode->GetSpeakerName();
    }

    return FText::GetEmpty();
}

FText UPBDialogueComponent::GetCurrentDialogueText() const
{
    if (CurrentNode)
    {
        return CurrentNode->GetDialogueText();
    }

    return FText::GetEmpty();
}

TArray<FText> UPBDialogueComponent::GetCurrentChoices() const
{
    TArray<FText> ChoiceTexts;

    if (CurrentNode)
    {
        TArray<FDialogueChoice> Choices = CurrentNode->GetChoices();
        for (const FDialogueChoice& Choice : Choices)
        {
            ChoiceTexts.Add(Choice.ChoiceText);
        }
    }

    return ChoiceTexts;
}

void UPBDialogueComponent::ProcessNodeEvents(UPBDialogueNode* Node)
{
    if (!Node)
    {
        return;
    }

    // Execute node events (give items, set flags, trigger animations, etc.)
    Node->ExecuteEvents(GetOwner());

    UE_LOG(LogPBDialogue, Verbose, TEXT("Processed events for node: %s"), *Node->GetName());
}

bool UPBDialogueComponent::CheckNodeConditions(UPBDialogueNode* Node) const
{
    if (!Node)
    {
        return false;
    }

    // Check if all conditions are met
    return Node->CheckConditions(GetOwner());
}