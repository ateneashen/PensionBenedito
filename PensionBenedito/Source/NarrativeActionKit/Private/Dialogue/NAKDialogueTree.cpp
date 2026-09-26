// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue system for narrative action games

#include "Dialogue/NAKDialogueTree.h"
#include "Dialogue/NAKDialogueNode.h"
#include "NarrativeActionKit.h"

UNAKDialogueTree::UNAKDialogueTree()
	: StartingNode(nullptr)
{
}

const TArray<UNAKDialogueNode*>& UNAKDialogueTree::GetNodes() const
{
	return Nodes;
}

UNAKDialogueNode* UNAKDialogueTree::GetStartingNode() const
{
	return StartingNode;
}

UNAKDialogueNode* UNAKDialogueTree::FindNodeBySpeaker(FName SpeakerID) const
{
	for (UNAKDialogueNode* Node : Nodes)
	{
		if (Node && Node->SpeakerName.ToString() == SpeakerID.ToString())
		{
			return Node;
		}
	}

	return nullptr;
}

bool UNAKDialogueTree::ValidateDialogueTree() const
{
	bool bIsValid = true;

	// Check: starting node exists
	if (!StartingNode)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("DialogueTree '%s': StartingNode is null."), *DialogueName.ToString());
		bIsValid = false;
	}

	// Check: nodes array is not empty
	if (Nodes.Num() == 0)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("DialogueTree '%s': Nodes array is empty."), *DialogueName.ToString());
		bIsValid = false;
	}

	// Check: starting node is in the nodes array
	if (StartingNode && !Nodes.Contains(StartingNode))
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("DialogueTree '%s': StartingNode is not in the Nodes array."),
			*DialogueName.ToString());
		bIsValid = false;
	}

	// Check: no null entries in nodes array
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		if (!Nodes[i])
		{
			UE_LOG(LogNAKDialogue, Warning, TEXT("DialogueTree '%s': Null node at index %d."),
				*DialogueName.ToString(), i);
			bIsValid = false;
			continue;
		}

		UNAKDialogueNode* Node = Nodes[i];

		// Check: non-end nodes must have either a next node or choices
		if (!Node->IsEndNode() && !Node->HasChoices() && !Node->NextNode)
		{
			UE_LOG(LogNAKDialogue, Warning,
				TEXT("DialogueTree '%s': Node '%s' is not an end node but has no NextNode or Choices."),
				*DialogueName.ToString(), *Node->GetSpeakerName().ToString());
			bIsValid = false;
		}

		// Check: choice references are valid
		for (const FNAKDialogueChoice& Choice : Node->GetChoices())
		{
			if (!Choice.NextNode)
			{
				UE_LOG(LogNAKDialogue, Warning,
					TEXT("DialogueTree '%s': Node '%s' has a choice '%s' with null NextNode."),
					*DialogueName.ToString(), *Node->GetSpeakerName().ToString(),
					*Choice.ChoiceText.ToString());
				bIsValid = false;
			}
		}
	}

	if (bIsValid)
	{
		UE_LOG(LogNAKDialogue, Log, TEXT("DialogueTree '%s': Validation passed. %d nodes."),
			*DialogueName.ToString(), Nodes.Num());
	}

	return bIsValid;
}

FPrimaryAssetId UNAKDialogueTree::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("Dialogue"), DialogueID);
}