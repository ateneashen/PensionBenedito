// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Character/NAKNPCCharacter.h"
#include "Core/Components/NAKDialogueComponent.h"
#include "Dialogue/NAKDialogueTree.h"
#include "NarrativeActionKit.h"

ANAKNPCCharacter::ANAKNPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create dialogue component
    DialogueComponent = CreateDefaultSubobject<UNAKDialogueComponent>(TEXT("DialogueComponent"));

    // Default values
    NPCName = FText::FromString("NPC");
    NPCDescription = FText::FromString("A person");
    DefaultDialogue = nullptr;
    GreetingDialogue = nullptr;
    bHasBeenSpokenTo = false;
    bIsInConversation = false;
}

void ANAKNPCCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Bind to dialogue end event
    if (DialogueComponent)
    {
        DialogueComponent->OnDialogueEnded.AddDynamic(this, &ANAKNPCCharacter::OnDialogueEnded);
    }
}

bool ANAKNPCCharacter::StartConversation(AActor* PlayerCharacter)
{
    if (bIsInConversation)
    {
        UE_LOG(LogNAKDialogue, Warning, TEXT("NPC %s is already in conversation"), *NPCName.ToString());
        return false;
    }

    UNAKDialogueTree* DialogueToUse = GetDialogueTree();
    if (!DialogueToUse)
    {
        UE_LOG(LogNAKDialogue, Warning, TEXT("NPC %s has no dialogue tree"), *NPCName.ToString());
        return false;
    }

    // Start dialogue
    if (DialogueComponent && DialogueComponent->StartDialogue(DialogueToUse))
    {
        bIsInConversation = true;
        bHasBeenSpokenTo = true;

        // Notify Blueprint
        OnConversationStarted(PlayerCharacter);

        UE_LOG(LogNAKDialogue, Log, TEXT("Conversation started with %s"), *NPCName.ToString());
        return true;
    }

    return false;
}

void ANAKNPCCharacter::EndConversation()
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

UNAKDialogueTree* ANAKNPCCharacter::GetDialogueTree_Implementation() const
{
    // Use greeting for first time, default after
    if (!bHasBeenSpokenTo && GreetingDialogue)
    {
        return GreetingDialogue;
    }

    return DefaultDialogue;
}

void ANAKNPCCharacter::OnDialogueEnded(UNAKDialogueTree* EndedTree)
{
    bIsInConversation = false;

    // Notify Blueprint
    AActor* Player = nullptr;
    OnConversationEnded(Player);

    UE_LOG(LogNAKDialogue, Log, TEXT("Conversation ended with %s"), *NPCName.ToString());
}