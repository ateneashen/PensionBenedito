// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Systems/PBNarrativeManager.h"
#include "PensionBenedito.h"

UPBNarrativeManager::UPBNarrativeManager()
{
}

void UPBNarrativeManager::Initialize()
{
    UE_LOG(LogPBNarrative, Log, TEXT("Narrative Manager initialized"));
}

void UPBNarrativeManager::SetFlag(FName FlagName, bool Value)
{
    bool* ExistingValue = NarrativeFlags.Find(FlagName);
    if (ExistingValue && *ExistingValue == Value)
    {
        return; // No change
    }

    NarrativeFlags.Add(FlagName, Value);
    OnFlagChanged.Broadcast(FlagName, Value);

    UE_LOG(LogPBNarrative, Verbose, TEXT("Flag '%s' set to %s"),
        *FlagName.ToString(), Value ? TEXT("true") : TEXT("false"));
}

bool UPBNarrativeManager::GetFlag(FName FlagName) const
{
    const bool* Value = NarrativeFlags.Find(FlagName);
    return Value ? *Value : false;
}

bool UPBNarrativeManager::HasFlag(FName FlagName) const
{
    return NarrativeFlags.Contains(FlagName);
}

void UPBNarrativeManager::StartQuest(FName QuestID)
{
    if (ActiveQuests.Contains(QuestID) || CompletedQuests.Contains(QuestID))
    {
        return;
    }

    ActiveQuests.Add(QuestID);
    OnQuestUpdated.Broadcast(QuestID);

    UE_LOG(LogPBNarrative, Log, TEXT("Quest started: %s"), *QuestID.ToString());
}

void UPBNarrativeManager::CompleteQuest(FName QuestID)
{
    if (!ActiveQuests.Contains(QuestID))
    {
        return;
    }

    ActiveQuests.Remove(QuestID);
    CompletedQuests.Add(QuestID);
    OnQuestUpdated.Broadcast(QuestID);

    UE_LOG(LogPBNarrative, Log, TEXT("Quest completed: %s"), *QuestID.ToString());
}

void UPBNarrativeManager::FailQuest(FName QuestID)
{
    if (!ActiveQuests.Contains(QuestID))
    {
        return;
    }

    ActiveQuests.Remove(QuestID);
    FailedQuests.Add(QuestID);
    OnQuestUpdated.Broadcast(QuestID);

    UE_LOG(LogPBNarrative, Log, TEXT("Quest failed: %s"), *QuestID.ToString());
}

bool UPBNarrativeManager::IsQuestActive(FName QuestID) const
{
    return ActiveQuests.Contains(QuestID);
}

bool UPBNarrativeManager::IsQuestCompleted(FName QuestID) const
{
    return CompletedQuests.Contains(QuestID);
}

void UPBNarrativeManager::RecordDiscovery(FName DiscoveryID)
{
    if (!Discoveries.Contains(DiscoveryID))
    {
        Discoveries.Add(DiscoveryID);
        UE_LOG(LogPBNarrative, Log, TEXT("Discovery recorded: %s"), *DiscoveryID.ToString());
    }
}

bool UPBNarrativeManager::HasDiscovery(FName DiscoveryID) const
{
    return Discoveries.Contains(DiscoveryID);
}

void UPBNarrativeManager::AddClue(FName ClueID, FText ClueDescription)
{
    if (!Clues.Contains(ClueID))
    {
        Clues.Add(ClueID, ClueDescription);
        UE_LOG(LogPBNarrative, Log, TEXT("Clue added: %s"), *ClueDescription.ToString());
    }
}

TArray<FText> UPBNarrativeManager::GetCollectedClues() const
{
    TArray<FText> Result;
    for (const auto& Pair : Clues)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

float UPBNarrativeManager::GetNarrativeProgress() const
{
    // Simple progress calculation based on discoveries and quests
    float Total = 100.0f;
    float Progress = 0.0f;

    // Each discovery adds progress
    Progress += Discoveries.Num() * 2.0f;

    // Each completed quest adds progress
    Progress += CompletedQuests.Num() * 10.0f;

    // Each clue adds progress
    Progress += Clues.Num() * 3.0f;

    return FMath::Clamp(Progress / Total, 0.0f, 1.0f);
}

FPNarrativeState UPBNarrativeManager::SaveState() const
{
    FPNarrativeState State;
    State.Flags = NarrativeFlags;
    State.Active = ActiveQuests;
    State.Completed = CompletedQuests;
    State.Failed = FailedQuests;
    State.Discovered = Discoveries;
    State.CluesFound = Clues;
    return State;
}

void UPBNarrativeManager::LoadState(const FPNarrativeState& State)
{
    NarrativeFlags = State.Flags;
    ActiveQuests = State.Active;
    CompletedQuests = State.Completed;
    FailedQuests = State.Failed;
    Discoveries = State.Discovered;
    Clues = State.CluesFound;

    UE_LOG(LogPBNarrative, Log, TEXT("Narrative state loaded"));
}