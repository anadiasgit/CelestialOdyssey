#include "CrystalGrowthAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCrystalGrowthAbility::UCrystalGrowthAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Initialize default values
    GrowthLevel = 1;
    CrystalDuration = 15.0f;
    MaxRange = 1000.0f;
    StructureType = ECrystalStructureType::Bridge;
}

void UCrystalGrowthAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!ActorInfo)
        return;

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
        return;

    // Add casting tag
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));

        // Apply cooldown
        if (CooldownEffectClass)
        {
            FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownEffectClass, GetAbilityLevel());
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }

    // Get target location based on aim
    FVector TargetLocation = GetTargetLocation(Character);

    // Calculate rotation based on surface normal (implement ray trace here)
    FRotator TargetRotation = FRotator::ZeroRotator; // Default to level

    // Spawn the crystal structure
    SpawnCrystalStructure(TargetLocation, TargetRotation);

    // Set timer to destroy crystals
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this]()
    {
        // TODO: Implement crystal cleanup logic
    },
    CrystalDuration,
    false
    );
}

void UCrystalGrowthAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UCrystalGrowthAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
        return false;

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.CrystalGrowth"))))
            return false;

        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
            return false;
    }

    return true;
}

FVector UCrystalGrowthAbility::GetTargetLocation(ACharacter* Character) const
{
    if (!Character)
        return FVector::ZeroVector;

    APlayerController* PC = Cast<APlayerController>(Character->GetController());
    if (!PC)
        return FVector::ZeroVector;

    // Get mouse cursor location in world space
    FVector WorldLocation, WorldDirection;
    if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        FHitResult HitResult;
        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + WorldDirection * MaxRange;

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Character);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            return HitResult.Location;
        }
    }

    return Character->GetActorLocation() + Character->GetActorForwardVector() * MaxRange;
}

void UCrystalGrowthAbility::SpawnCrystalStructure(const FVector& Location, const FRotator& Rotation)
{
    // This would be implemented in Blueprint to spawn the actual crystal mesh/actor
    // The implementation depends on your specific crystal structure assets

    // Different structure types and complexity based on growth level
    switch (GrowthLevel)
    {
    case 1:
        // Basic structures only
        if (StructureType != ECrystalStructureType::Bridge &&
            StructureType != ECrystalStructureType::Platform)
        {
            StructureType = ECrystalStructureType::Bridge;
        }
        break;

    case 2:
        // Add more complex structures
        // Implementation for spiral staircases and arches
        break;

    case 3:
        // All structure types available
        // Add encasement functionality
        break;
    }
}