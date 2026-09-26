// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games

#include "Core/Character/NAKCharacterBase.h"
#include "Core/Components/NAKInteractionComponent.h"
#include "Core/Components/NAKInventoryComponent.h"
#include "Core/Components/NAKSanityComponent.h"
#include "Core/Components/NAKAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NarrativeActionKit.h"

ANAKCharacterBase::ANAKCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Ability System Component
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    // Create Attribute Set
    AttributeSet = CreateDefaultSubobject<UNAKAttributeSet>(TEXT("AttributeSet"));

    // Create Interaction Component
    InteractionComponent = CreateDefaultSubobject<UNAKInteractionComponent>(TEXT("InteractionComponent"));

    // Create Inventory Component
    InventoryComponent = CreateDefaultSubobject<UNAKInventoryComponent>(TEXT("InventoryComponent"));

    // Create Sanity Component
    SanityComponent = CreateDefaultSubobject<UNAKSanityComponent>(TEXT("SanityComponent"));
}

UAbilitySystemComponent* ANAKCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

bool ANAKCharacterBase::IsAlive() const
{
    if (AttributeSet)
    {
        return AttributeSet->GetHealth() > 0.0f;
    }
    return false;
}

float ANAKCharacterBase::GetCurrentHealth() const
{
    if (AttributeSet)
    {
        return AttributeSet->GetHealth();
    }
    return 0.0f;
}

float ANAKCharacterBase::GetMaxHealth() const
{
    if (AttributeSet)
    {
        return AttributeSet->GetMaxHealth();
    }
    return 0.0f;
}

void ANAKCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void ANAKCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Initialize ability system on server
    InitializeAbilitySystem();
}

void ANAKCharacterBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Initialize ability system on client
    InitializeAbilitySystem();
}

void ANAKCharacterBase::InitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogNAK, Error, TEXT("InitializeAbilitySystem: AbilitySystemComponent is null on %s"), *GetName());
        return;
    }

    // Apply default tags
    if (DefaultTags.Num() > 0)
    {
        AbilitySystemComponent->AddLooseGameplayTags(DefaultTags);
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

    UE_LOG(LogNAK, Log, TEXT("Ability system initialized for %s with %d abilities and %d effects"),
        *GetName(), DefaultAbilities.Num(), DefaultEffects.Num());
}