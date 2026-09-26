// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue component for narrative action games

#include "Core/Components/NAKDialogueComponent.h"
#include "Dialogue/NAKDialogueTree.h"
#include "Dialogue/NAKDialogueNode.h"
#include "NarrativeActionKit.h"

// Static empty array returned when no dialogue is active
const TArray<FNAKDialogueChoice> UNAKDialogueComponent::EmptyChoices;

UNAKDialogueComponent::UNAKDialogueComponent()
	: CurrentDialogueTree(nullptr)
	, CurrentNode(nullptr)
	, CurrentNodeIndex(INDEX_NONE)
	, bIsDialogueActive(false)
{
	// Dialogue component does not need to tick
	PrimaryComponentTick.bCanEverTick = false;
}

void UNAKDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNAKDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// End any active dialogue before destruction
	if (bIsDialogueActive)
	{
		EndDialogue();
	}

	Super::EndPlay(EndPlayReason);
}

void UNAKDialogueComponent::StartDialogue(UNAKDialogueTree* DialogueTree)
{
	if (bIsDialogueActive)
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("StartDialogue: Dialogue already active. End current dialogue before starting a new one."));
		return;
	}

	if (!DialogueTree)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("StartDialogue: DialogueTree is null."));
		return;
	}

	if (!DialogueTree->GetStartingNode())
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("StartDialogue: DialogueTree '%s' has no StartingNode."),
			*DialogueTree->DialogueName.ToString());
		return;
	}

	CurrentDialogueTree = DialogueTree;
	bIsDialogueActive = true;

	// Navigate to the starting node
	SetCurrentNode(DialogueTree->GetStartingNode());

	UE_LOG(LogNAKDialogue, Log, TEXT("Dialogue started: '%s'"),
		*DialogueTree->DialogueName.ToString());

	OnDialogueStarted.Broadcast(CurrentDialogueTree);
}

void UNAKDialogueComponent::EndDialogue()
{
	if (!bIsDialogueActive)
	{
		return;
	}

	UNAKDialogueTree* EndingTree = CurrentDialogueTree;

	UE_LOG(LogNAKDialogue, Log, TEXT("Dialogue ended: '%s'"),
		*CurrentDialogueTree->DialogueName.ToString());

	// Clear state
	CurrentDialogueTree = nullptr;
	CurrentNode = nullptr;
	CurrentNodeIndex = INDEX_NONE;
	bIsDialogueActive = false;

	OnDialogueEnded.Broadcast(EndingTree);
}

void UNAKDialogueComponent::ProgressDialogue()
{
	if (!bIsDialogueActive)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("ProgressDialogue: No active dialogue."));
		return;
	}

	if (!CurrentNode)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("ProgressDialogue: CurrentNode is null."));
		return;
	}

	// Cannot progress from a node with choices — player must pick one
	if (CurrentNode->HasChoices())
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("ProgressDialogue: Current node has choices. Use MakeChoice instead."));
		return;
	}

	// End dialogue if current node is an end node
	if (CurrentNode->IsEndNode())
	{
		EndDialogue();
		return;
	}

	// Advance to the next node
	UNAKDialogueNode* NextNode = CurrentNode->GetNextNode();
	if (!NextNode)
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("ProgressDialogue: Current node '%s' has no NextNode. Ending dialogue."),
			*CurrentNode->GetSpeakerName().ToString());
		EndDialogue();
		return;
	}

	SetCurrentNode(NextNode);

	// If we landed on an end node, end the dialogue
	if (CurrentNode->IsEndNode())
	{
		EndDialogue();
	}
}

void UNAKDialogueComponent::MakeChoice(int32 ChoiceIndex)
{
	if (!bIsDialogueActive)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("MakeChoice: No active dialogue."));
		return;
	}

	if (!CurrentNode)
	{
		UE_LOG(LogNAKDialogue, Warning, TEXT("MakeChoice: CurrentNode is null."));
		return;
	}

	const TArray<FNAKDialogueChoice>& Choices = CurrentNode->GetChoices();
	if (!Choices.IsValidIndex(ChoiceIndex))
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("MakeChoice: Invalid ChoiceIndex %d. Node '%s' has %d choices."),
			ChoiceIndex, *CurrentNode->GetSpeakerName().ToString(), Choices.Num());
		return;
	}

	const FNAKDialogueChoice& SelectedChoice = Choices[ChoiceIndex];

	if (!SelectedChoice.NextNode)
	{
		UE_LOG(LogNAKDialogue, Warning,
			TEXT("MakeChoice: Choice '%s' at index %d has null NextNode."),
			*SelectedChoice.ChoiceText.ToString(), ChoiceIndex);
		EndDialogue();
		return;
	}

	UE_LOG(LogNAKDialogue, Log, TEXT("Choice made: '%s' (index %d)"),
		*SelectedChoice.ChoiceText.ToString(), ChoiceIndex);

	OnDialogueChoiceMade.Broadcast(ChoiceIndex);

	SetCurrentNode(SelectedChoice.NextNode);

	// If we landed on an end node, end the dialogue
	if (CurrentNode->IsEndNode())
	{
		EndDialogue();
	}
}

bool UNAKDialogueComponent::IsDialogueActive() const
{
	return bIsDialogueActive;
}

UNAKDialogueNode* UNAKDialogueComponent::GetCurrentNode() const
{
	return CurrentNode;
}

FText UNAKDialogueComponent::GetCurrentSpeakerName() const
{
	if (CurrentNode)
	{
		return CurrentNode->GetSpeakerName();
	}

	return FText::GetEmpty();
}

FText UNAKDialogueComponent::GetCurrentDialogueText() const
{
	if (CurrentNode)
	{
		return CurrentNode->GetDialogueText();
	}

	return FText::GetEmpty();
}

const TArray<FNAKDialogueChoice>& UNAKDialogueComponent::GetCurrentChoices() const
{
	if (CurrentNode)
	{
		return CurrentNode->GetChoices();
	}

	return EmptyChoices;
}

void UNAKDialogueComponent::SetCurrentNode(UNAKDialogueNode* NewNode)
{
	CurrentNode = NewNode;

	if (CurrentDialogueTree && NewNode)
	{
		CurrentNodeIndex = CurrentDialogueTree->GetNodes().IndexOfByKey(NewNode);
	}
	else
	{
		CurrentNodeIndex = INDEX_NONE;
	}

	OnDialogueNodeChanged.Broadcast(CurrentNode, CurrentNodeIndex);
}