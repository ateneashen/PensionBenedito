// Copyright Pension Benedito, 2024. All rights reserved.

#include "Core/Character/PBCharacterBase.h"
#include "Core/Components/PBInteractionComponent.h"
#include "Core/Components/PBInventoryComponent.h"
#include "Core/Components/PBSanityComponent.h"
#include "AbilitySystemComponent.h"
#include "PensionBenedito.h"

APBCharacterBase::APBCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Ability System Component
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    // Create Attribute Set (will be created automatically by ASC)
    AttributeSet = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSet"));

    // Create Interaction Component
    InteractionComponent = CreateDefaultSubobject<UPBInteractionComponent>(TEXT("InteractionComponent"));

    // Create Inventory Component
    InventoryComponent = CreateDefaultSubobject<UPBInventoryComponent>(TEXT("InventoryComponent"));

    // Create Sanity Component
    SanityComponent = CreateDefaultSubobject<UPBSanityComponent>(TEXT("SanityComponent"));
}

UAbilitySystemComponent* APBCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void APBCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void APBCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Initialize ability system on server
    InitializeAbilitySystem();
}

void APBCharacterBase::InitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogPB, Error, TEXT("InitializeAbilitySystem: AbilitySystemComponent is null on %s"), *GetName());
        return;
    }

    // Grant default abilities
    for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (AbilityClass)
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
            AbilitySystemComponent->GiveAbility(AbilitySpec);
        }
    }

    // Apply default effects
    for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultEffects)
    {
        if (EffectClass)
        {
            FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
            EffectContext.AddSourceObject(this);

            FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, EffectContext);
            if (EffectSpec.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
            }
        }
    }

    UE_LOG(LogPB, Log, TEXT("Ability system initialized for %s with %d abilities and %d effects"),
        *GetName(), DefaultAbilities.Num(), DefaultEffects.Num());
}