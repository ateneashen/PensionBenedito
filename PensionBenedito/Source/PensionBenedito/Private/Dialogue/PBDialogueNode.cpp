// Copyright Pension Benedito, 2024. All rights reserved.

#include "Dialogue/PBDialogueNode.h"
#include "Dialogue/PBDialogueCondition.h"
#include "Dialogue/PBDialogueEvent.h"
#include "PensionBenedito.h"

UPBDialogueNode::UPBDialogueNode()
{
    SpeakerName = FText::FromString("Speaker");
    SpeakerPortrait = nullptr;
    SpeakerID = NAME_None;
    DialogueText = FText::FromString("Dialogue text");
    DialogueTextES = FText::GetEmpty();
    bIsPlayerNode = false;
    bIsEndNode = false;
    AutoAdvanceDelay = 0.0f;
    NextNode = nullptr;
}

FText UPBDialogueNode::GetDialogueText() const
{
    // TODO: Check current language and return appropriate text
    // For now, return English text
    return DialogueText;
}

bool UPBDialogueNode::CheckConditions(AActor* Context) const
{
    // Check all conditions
    for (const UPBDialogueCondition* Condition : NodeConditions)
    {
        if (Condition && !Condition->CheckCondition(Context))
        {
            return false;
        }
    }

    return true;
}

void UPBDialogueNode::ExecuteEvents(AActor* Context)
{
    // Execute all events
    for (UPBDialogueEvent* Event : NodeEvents)
    {
        if (Event)
        {
            Event->ExecuteEvent(Context);
        }
    }
}