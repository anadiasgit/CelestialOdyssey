#include "GravityShiftAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

/** Default constructor for UGravityShiftAbility */
UGravityShiftAbility::UGravityShiftAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set default level and gravity shift duration
    GravityShiftLevel = 1;
    GravityShiftDuration = 5.0f;
}

/**
 * @brief Activates the gravity shift ability.
 *
 * This function inverts gravity, rotates the player, and lets them float upwards.
 * After the specified duration, gravity reverts back to normal.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor activating the ability.
 * @param ActivationInfo Information about the activation context.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UGravityShiftAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Set gravity inversion duration based on ability level
        switch (GravityShiftLevel)
        {
        case 1:
            GravityShiftDuration = 5.0f;
            break;
        case 2:
            GravityShiftDuration = 10.0f;
            break;
        case 3:
            GravityShiftDuration = 15.0f;
            break;
        default:
            GravityShiftDuration = 5.0f; // Default to level 1
            break;
        }

        // Invert gravity by multiplying by -1
        Character->GetCharacterMovement()->GravityScale = -1.0f;

        // Rotate the character to match inverted gravity
        RotateCharacter(Character, true);

        // Set a timer to revert gravity after the specified duration
        FTimerHandle TimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateUObject(this, &UGravityShiftAbility::RevertGravity, Character),
            GravityShiftDuration,
            false);
    }
}

/**
 * @brief Ends the gravity shift ability, reverting gravity back to normal.
 *
 * @param Handle Handle that identifies the ability being ended.
 * @param ActorInfo Information about the actor that activated the ability.
 * @param ActivationInfo Information about the context of the ability ending.
 * @param bReplicateEndAbility Should replication of the ability's end occur.
 * @param bWasCancelled Was the ability manually canceled.
 */
void UGravityShiftAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }
}

/**
 * @brief Reverts the gravity back to normal.
 *
 * This function resets gravity and rotates the character to the correct orientation.
 *
 * @param Character The character whose gravity is being reverted.
 */
void UGravityShiftAbility::RevertGravity(ACharacter* Character)
{
    if (Character)
    {
        // Revert gravity back to normal
        Character->GetCharacterMovement()->GravityScale = 1.0f;

        // Rotate character back to original orientation
        RotateCharacter(Character, false);
    }
}

/**
 * @brief Rotates the character to align with gravity shift.
 *
 * This function handles rotating the character when gravity is inverted and resetting their rotation when gravity is reverted.
 *
 * @param Character The character to be rotated.
 * @param bIsGravityInverted Boolean indicating if gravity is inverted.
 */
void UGravityShiftAbility::RotateCharacter(ACharacter* Character, bool bIsGravityInverted)
{
    if (Character)
    {
        FRotator NewRotation = Character->GetActorRotation();
        if (bIsGravityInverted)
        {
            // Rotate 180 degrees to make the character's feet point upwards
            NewRotation.Roll = 180.0f;
        }
        else
        {
            // Set back to normal orientation
            NewRotation.Roll = 0.0f;
        }
        Character->SetActorRotation(NewRotation);
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
bool UGravityShiftAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
            return false; // Prevent ability activation if on cooldown or casting another ability
        }
    }

    return true;
}
