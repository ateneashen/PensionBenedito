// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Character/PBPlayerCharacter.h"
#include "Core/Components/PBDialogueComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PensionBenedito.h"

APBPlayerCharacter::APBPlayerCharacter()
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

    // Dialogue component (player-specific)
    DialogueComponent = CreateDefaultSubobject<UPBDialogueComponent>(TEXT("DialogueComponent"));

    // Input settings
    BaseTurnRate = 45.0f;
    BaseLookUpRate = 45.0f;

    // Movement settings for the period (slower, more realistic)
    GetCharacterMovement()->MaxWalkSpeed = 350.0f;
    GetCharacterMovement()->JumpZVelocity = 400.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Dialogue state
    bIsInDialogue = false;
}

void APBPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Axis mappings
    PlayerInputComponent->BindAxis("MoveForward", this, &APBPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APBPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APBPlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APBPlayerCharacter::LookUpAtRate);

    // Action mappings
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APBPlayerCharacter::Interact);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APBPlayerCharacter::OpenInventory);
    PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APBPlayerCharacter::PauseGame);
}

void APBPlayerCharacter::MoveForward(float Value)
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

void APBPlayerCharacter::MoveRight(float Value)
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

void APBPlayerCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APBPlayerCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APBPlayerCharacter::Interact()
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

void APBPlayerCharacter::OpenInventory()
{
    if (bIsInDialogue)
    {
        return;
    }

    // Toggle inventory UI
    UE_LOG(LogPBInteraction, Log, TEXT("Inventory toggled"));
    // TODO: Implement inventory UI
}

void APBPlayerCharacter::PauseGame()
{
    // TODO: Implement pause menu
    UE_LOG(LogPB, Log, TEXT("Game paused"));
}

APlayerController* APBPlayerCharacter::GetPBPlayerController() const
{
    return Cast<APlayerController>(GetController());
}

void APBPlayerCharacter::SetDialogueState(bool bInDialogue)
{
    bIsInDialogue = bInDialogue;

    // Disable movement during dialogue
    if (bInDialogue)
    {
        GetCharacterMovement()->DisableMovement();
    }
    else
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }

    UE_LOG(LogPBDialogue, Log, TEXT("Dialogue state changed: %s"), bInDialogue ? TEXT("Active") : TEXT("Inactive"));
}

void APBPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}