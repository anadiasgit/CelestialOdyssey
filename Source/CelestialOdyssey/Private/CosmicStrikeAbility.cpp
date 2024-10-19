#include "CosmicStrikeAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

/** Default constructor for UCosmicStrikeAbility */
UCosmicStrikeAbility::UCosmicStrikeAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set default combo level
    ComboLevel = 1;
}

/**
 * @brief Activates the Cosmic Strike ability.
 *
 * This function executes the logic for performing a melee combo attack.
 * Depending on the combo level, different effects and attack counts are applied.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UCosmicStrikeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // Add the casting tag
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        ExecuteCombo(Character);

        // TODO: Implement proper cooldown using Gameplay Effects and Tags
    }

    // End the ability after activation
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

/**
 * @brief Executes the combo attack logic.
 *
 * This function handles the melee combo sequence, including striking enemies,
 * applying knockback, and triggering additional effects based on progression level.
 *
 * @param Character The character performing the combo attack.
 */
void UCosmicStrikeAbility::ExecuteCombo(ACharacter* Character)
{
    if (!Character)
    {
        return;
    }

    // Implement combo logic based on ability level
    switch (ComboLevel)
    {
    case 1: // Level 1: Basic three-hit combo
        UE_LOG(LogTemp, Log, TEXT("Performing combo hit 1"));
        // TODO: Implement hit animation and detection logic
        break;

    case 2: // Level 2: Adds knockback to the final hit
        UE_LOG(LogTemp, Log, TEXT("Performing combo hit with knockback"));
        // TODO: Implement knockback effect
        break;

    case 3: // Level 3: Releases an energy wave on the final hit
        UE_LOG(LogTemp, Log, TEXT("Performing combo hit with energy wave"));
        // TODO: Implement energy wave effect
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown combo level"));
        break;
    }

    // TODO: Properly handle the combo state and cooldown using GAS features
}

/**
 * @brief Checks if the ability can be activated.
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
bool UCosmicStrikeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // Check tags in Ability System Component (ASC)
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        // Prevent ability activation during cooldown or if another ability is being cast
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.OnCooldown"))) ||
            ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }

    return true;
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
void UCosmicStrikeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    // Remove the casting tag
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }
}
