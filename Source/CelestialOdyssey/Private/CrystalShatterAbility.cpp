#include "CrystalShatterAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

/** Default constructor for UCrystalShatterAbility */
UCrystalShatterAbility::UCrystalShatterAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set defaults
    ShatterLevel = 1;
    ShardDuration = 5.0f;
}

/**
 * @brief Activates the Crystal Shatter ability.
 *
 * This function executes the logic for shattering nearby crystals, sending sharp fragments flying.
 * Depending on the shatter level, different effects are applied, including ricocheting and DoT.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UCrystalShatterAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        FVector ShatterLocation = Character->GetActorLocation();

        // Perform different actions based on shatter level
        switch (ShatterLevel)
        {
        case 1:
            UE_LOG(LogTemp, Log, TEXT("Performing basic crystal shatter at %s"), *ShatterLocation.ToString());
            // TODO: Implement basic shatter logic with moderate damage
            break;

        case 2:
            UE_LOG(LogTemp, Log, TEXT("Performing crystal shatter with DoT at %s"), *ShatterLocation.ToString());
            // TODO: Implement DoT effect on shards
            break;

        case 3:
            UE_LOG(LogTemp, Log, TEXT("Performing crystal shatter with ricochet at %s"), *ShatterLocation.ToString());
            // TODO: Implement ricochet logic for fragments
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown shatter level"));
            break;
        }

        // Set a timer to remove the shards after ShardDuration
        FTimerHandle ShardTimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            ShardTimerHandle,
            [this, ShatterLocation]()
        {
            // TODO: Implement logic to remove shards after duration ends
            UE_LOG(LogTemp, Log, TEXT("Shards at %s removed."), *ShatterLocation.ToString());
        },
            ShardDuration,
            false);
    }

    // End the ability after activation
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

/**
 * @brief Ends the ability and cleans up.
 *
 * @param Handle Handle that identifies the ability being ended.
 * @param ActorInfo Information about the actor that activated the ability.
 * @param ActivationInfo Information about the context of the ability ending.
 * @param bReplicateEndAbility Whether to replicate the end ability event.
 * @param bWasCancelled Whether the ability was cancelled.
 */
void UCrystalShatterAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }
}

/**
 * @brief Determines if the ability can be activated.
 *
 * This function checks the Ability System Component for active tags that would block this ability.
 *
 * @param Handle The handle to this specific instance of the ability.
 * @param ActorInfo Information about the actor attempting to activate the ability.
 * @param SourceTags Source tags used for additional checks.
 * @param TargetTags Target tags used for additional checks.
 * @param OptionalRelevantTags Tags that are optionally relevant to this ability.
 * @return True if the ability can be activated, false otherwise.
 */
bool UCrystalShatterAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // Check tags in Ability System Component (ASC)
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.OnCooldown"))) ||
            ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false; // Prevent ability activation during cooldown or when casting another ability
        }
    }

    return true;
}
