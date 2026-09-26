// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Character/NAKPlayerCharacter.h"
#include "Core/Components/NAKDialogueComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "NarrativeActionKit.h"

ANAKPlayerCharacter::ANAKPlayerCharacter()
{
    // Camera boom (3rd person)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 5.0f;

    // Follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Dialogue component
    DialogueComponent = CreateDefaultSubobject<UNAKDialogueComponent>(TEXT("DialogueComponent"));

    // Input settings
    BaseTurnRate = 45.0f;
    BaseLookUpRate = 45.0f;

    // Movement settings
    GetCharacterMovement()->MaxWalkSpeed = 350.0f;
    GetCharacterMovement()->JumpZVelocity = 400.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Dialogue state
    bIsInDialogue = false;
}

void ANAKPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ANAKPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Axis mappings (classic UE4.27)
    PlayerInputComponent->BindAxis("MoveForward", this, &ANAKPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ANAKPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ANAKPlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &ANAKPlayerCharacter::LookUpAtRate);

    // Action mappings
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ANAKPlayerCharacter::Interact);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ANAKPlayerCharacter::OpenInventory);
    PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ANAKPlayerCharacter::PauseGame);
}

void ANAKPlayerCharacter::MoveForward(float Value)
{
    if (bIsInDialogue || Value == 0.0f)
    {
        return;
    }

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ANAKPlayerCharacter::MoveRight(float Value)
{
    if (bIsInDialogue || Value == 0.0f)
    {
        return;
    }

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ANAKPlayerCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANAKPlayerCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANAKPlayerCharacter::Interact()
{
    if (bIsInDialogue)
    {
        // Progress dialogue
        if (DialogueComponent)
        {
            DialogueComponent->ProgressDialogue();
        }
        return;
    }

    // Try to interact with nearby interactable
    if (InteractionComponent)
    {
        InteractionComponent->TryInteract();
    }
}

void ANAKPlayerCharacter::OpenInventory()
{
    if (bIsInDialogue)
    {
        return;
    }

    // Toggle inventory UI - override in game-specific class
    UE_LOG(LogNAKInteraction, Log, TEXT("Inventory toggled"));
}

void ANAKPlayerCharacter::PauseGame()
{
    // Override in game-specific class
    UE_LOG(LogNAK, Log, TEXT("Game paused"));
}

APlayerController* ANAKPlayerCharacter::GetNAKPlayerController() const
{
    return Cast<APlayerController>(GetController());
}

void ANAKPlayerCharacter::SetDialogueState(bool bInDialogue)
{
    bIsInDialogue = bInDialogue;

    if (bInDialogue)
    {
        GetCharacterMovement()->DisableMovement();
    }
    else
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }

    UE_LOG(LogNAKDialogue, Log, TEXT("Dialogue state changed: %s"), bInDialogue ? TEXT("Active") : TEXT("Inactive"));
}

void ANAKPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}