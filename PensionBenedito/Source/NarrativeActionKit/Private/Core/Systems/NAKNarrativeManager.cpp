// Copyright 2024 NarrativeActionKit. All Rights Reserved.

#include "Core/Systems/NAKNarrativeManager.h"

DEFINE_LOG_CATEGORY(LogNAKNarrative);

UNAKNarrativeManager::UNAKNarrativeManager()
{
	TotalNarrativeElements = 0;
}

void UNAKNarrativeManager::Initialize()
{
	UE_LOG(LogNAKNarrative, Log, TEXT("NarrativeManager initialized"));
}

// ===== Flag Functions =====

void UNAKNarrativeManager::SetFlag(FName FlagName, bool bValue)
{
	bool* ExistingValue = NarrativeState.Flags.Find(FlagName);
	bool bChanged = (ExistingValue == nullptr) || (*ExistingValue != bValue);

	NarrativeState.Flags.Add(FlagName, bValue);

	if (bChanged)
	{
		OnFlagChanged.Broadcast(FlagName, bValue);
		UE_LOG(LogNAKNarrative, Log, TEXT("Flag '%s' set to %s"), *FlagName.ToString(), bValue ? TEXT("true") : TEXT("false"));
	}
}

bool UNAKNarrativeManager::GetFlag(FName FlagName) const
{
	const bool* Value = NarrativeState.Flags.Find(FlagName);
	return Value ? *Value : false;
}

bool UNAKNarrativeManager::HasFlag(FName FlagName) const
{
	return NarrativeState.Flags.Contains(FlagName);
}

// ===== Quest Functions =====

void UNAKNarrativeManager::StartQuest(FName QuestID)
{
	if (!NarrativeState.ActiveQuests.Contains(QuestID) && !NarrativeState.CompletedQuests.Contains(QuestID))
	{
		NarrativeState.ActiveQuests.Add(QuestID);
		OnQuestUpdated.Broadcast(QuestID);
		UE_LOG(LogNAKNarrative, Log, TEXT("Quest '%s' started"), *QuestID.ToString());
	}
}

void UNAKNarrativeManager::CompleteQuest(FName QuestID)
{
	if (NarrativeState.ActiveQuests.Contains(QuestID))
	{
		NarrativeState.ActiveQuests.Remove(QuestID);
		NarrativeState.CompletedQuests.Add(QuestID);
		OnQuestUpdated.Broadcast(QuestID);
		UE_LOG(LogNAKNarrative, Log, TEXT("Quest '%s' completed"), *QuestID.ToString());
	}
}

void UNAKNarrativeManager::FailQuest(FName QuestID)
{
	if (NarrativeState.ActiveQuests.Contains(QuestID))
	{
		NarrativeState.ActiveQuests.Remove(QuestID);
		NarrativeState.FailedQuests.Add(QuestID);
		OnQuestUpdated.Broadcast(QuestID);
		UE_LOG(LogNAKNarrative, Log, TEXT("Quest '%s' failed"), *QuestID.ToString());
	}
}

bool UNAKNarrativeManager::IsQuestActive(FName QuestID) const
{
	return NarrativeState.ActiveQuests.Contains(QuestID);
}

bool UNAKNarrativeManager::IsQuestCompleted(FName QuestID) const
{
	return NarrativeState.CompletedQuests.Contains(QuestID);
}

// ===== Discovery Functions =====

void UNAKNarrativeManager::RecordDiscovery(FName DiscoveryID)
{
	if (!NarrativeState.Discoveries.Contains(DiscoveryID))
	{
		NarrativeState.Discoveries.Add(DiscoveryID);
		UE_LOG(LogNAKNarrative, Log, TEXT("Discovery '%s' recorded"), *DiscoveryID.ToString());
	}
}

bool UNAKNarrativeManager::HasDiscovery(FName DiscoveryID) const
{
	return NarrativeState.Discoveries.Contains(DiscoveryID);
}

// ===== Clue Functions =====

void UNAKNarrativeManager::AddClue(FName ClueID, const FText& Description)
{
	if (!NarrativeState.Clues.Contains(ClueID))
	{
		NarrativeState.Clues.Add(ClueID, Description);
		UE_LOG(LogNAKNarrative, Log, TEXT("Clue '%s' added"), *ClueID.ToString());
	}
}

TMap<FName, FText> UNAKNarrativeManager::GetCollectedClues() const
{
	return NarrativeState.Clues;
}

// ===== Save/Load Functions =====

FNAKNarrativeState UNAKNarrativeManager::SaveState() const
{
	return NarrativeState;
}

void UNAKNarrativeManager::LoadState(const FNAKNarrativeState& State)
{
	NarrativeState = State;
	UE_LOG(LogNAKNarrative, Log, TEXT("Narrative state loaded"));
}

// ===== Progress Functions =====

float UNAKNarrativeManager::GetNarrativeProgress() const
{
	int32 CompletedElements = 0;
	int32 TotalElements = 0;

	// Count quests (completed vs total active + completed + failed)
	int32 TotalQuests = NarrativeState.ActiveQuests.Num() + NarrativeState.CompletedQuests.Num() + NarrativeState.FailedQuests.Num();
	if (TotalQuests > 0)
	{
		CompletedElements += NarrativeState.CompletedQuests.Num();
		TotalElements += TotalQuests;
	}

	// Count discoveries
	int32 TotalDiscoveries = NarrativeState.Discoveries.Num();
	if (TotalDiscoveries > 0)
	{
		CompletedElements += TotalDiscoveries;
		TotalElements += TotalDiscoveries;
	}

	// Count clues
	int32 TotalClues = NarrativeState.Clues.Num();
	if (TotalClues > 0)
	{
		CompletedElements += TotalClues;
		TotalElements += TotalClues;
	}

	// Avoid division by zero
	if (TotalElements == 0)
	{
		return 0.0f;
	}

	return static_cast<float>(CompletedElements) / static_cast<float>(TotalElements);
}