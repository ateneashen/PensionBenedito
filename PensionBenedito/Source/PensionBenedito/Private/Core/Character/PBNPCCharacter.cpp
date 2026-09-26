// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Character/PBNPCCharacter.h"
#include "Core/Components/PBDialogueComponent.h"
#include "Dialogue/PBDialogueTree.h"
#include "PensionBenedito.h"

APBNPCCharacter::APBNPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create dialogue component
    DialogueComponent = CreateDefaultSubobject<UPBDialogueComponent>(TEXT("DialogueComponent"));

    // Default values
    NPCName = FText::FromString("NPC");
    NPCDescription = FText::FromString("A person");
    DefaultDialogue = nullptr;
    GreetingDialogue = nullptr;
    bHasBeenSpokenTo = false;
    bIsInConversation = false;
}

void APBNPCCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Bind to dialogue end event
    if (DialogueComponent)
    {
        DialogueComponent->OnDialogueEnded.AddDynamic(this, &APBNPCCharacter::OnDialogueEnded);
    }
}

bool APBNPCCharacter::StartConversation(AActor* PlayerCharacter)
{
    if (bIsInConversation)
    {
        UE_LOG(LogPBDialogue, Warning, TEXT("NPC %s is already in conversation"), *NPCName.ToString());
        return false;
    }

    UPBDialogueTree* DialogueToUse = GetDialogueTree();
    if (!DialogueToUse)
    {
        UE_LOG(LogPBDialogue, Warning, TEXT("NPC %s has no dialogue tree"), *NPCName.ToString());
        return false;
    }

    // Start dialogue
    if (DialogueComponent && DialogueComponent->StartDialogue(DialogueToUse))
    {
        bIsInConversation = true;
        bHasBeenSpokenTo = true;

        // Notify Blueprint
        OnConversationStarted(PlayerCharacter);

        UE_LOG(LogPBDialogue, Log, TEXT("Conversation started with %s"), *NPCName.ToString());
        return true;
    }

    return false;
}

void APBNPCCharacter::EndConversation()
{
    if (!bIsInConversation)
    {
        return;
    }

    if (DialogueComponent)
    {
        DialogueComponent->EndDialogue();
    }

    bIsInConversation = false;
}

UPBDialogueTree* APBNPCCharacter::GetDialogueTree_Implementation() const
{
    // Use greeting for first time, default after
    if (!bHasBeenSpokenTo && GreetingDialogue)
    {
        return GreetingDialogue;
    }

    return DefaultDialogue;
}

void APBNPCCharacter::OnDialogueEnded(UPBDialogueTree* EndedTree)
{
    bIsInConversation = false;

    // Notify Blueprint
    AActor* Player = nullptr;
    // TODO: Get player reference
    OnConversationEnded(Player);

    UE_LOG(LogPBDialogue, Log, TEXT("Conversation ended with %s"), *NPCName.ToString());
}