// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable dialogue system for narrative action games

#include "Dialogue/NAKDialogueNode.h"

UNAKDialogueNode::UNAKDialogueNode()
	: bIsPlayerNode(false)
	, bIsEndNode(false)
	, NextNode(nullptr)
{
}

FText UNAKDialogueNode::GetDialogueText() const
{
	return DialogueText;
}

FText UNAKDialogueNode::GetSpeakerName() const
{
	return SpeakerName;
}

UNAKDialogueNode* UNAKDialogueNode::GetNextNode() const
{
	if (bIsEndNode || HasChoices())
	{
		return nullptr;
	}

	return NextNode;
}

const TArray<FNAKDialogueChoice>& UNAKDialogueNode::GetChoices() const
{
	return Choices;
}

bool UNAKDialogueNode::HasChoices() const
{
	return Choices.Num() > 0;
}

bool UNAKDialogueNode::IsEndNode() const
{
	return bIsEndNode;
}