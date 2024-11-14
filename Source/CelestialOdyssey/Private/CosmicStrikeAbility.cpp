#include "CosmicStrikeAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

/** Default constructor for UCosmicStrikeAbility */
UCosmicStrikeAbility::UCosmicStrikeAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Initialize ability properties
    AbilityLevel = 1; // Default level
    AttackCooldown = 0.5f; // 0.5 seconds cooldown between each attack
}

/**
 * @brief Activates the Cosmic Strike ability.
 */
void UCosmicStrikeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Perform the attack based on the current ability level
        PerformAttack(Character);

        // Set cooldown timer
        FTimerManager& TimerManager = Character->GetWorldTimerManager();

        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("EndComboAttack"));

        TimerManager.SetTimer(
            ComboResetTimerHandle,
            TimerDelegate,
            AttackCooldown,
            false
        );
    }
}

/**
 * @brief Helper function to end the combo attack ability.
 */
void UCosmicStrikeAbility::EndComboAttack()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


/**
 * @brief Ends the Cosmic Strike ability.
 */
void UCosmicStrikeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @brief Determines if the ability can be activated.
 */
bool UCosmicStrikeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.CosmicStrike"))))
        {
            return false;
        }

        // Check if any ability is currently casting
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }
    // Ensure the character can activate the ability
    return true;
}

/**
 * @brief Performs the attack based on the current level of the ability.
 */
void UCosmicStrikeAbility::PerformAttack(ACharacter* Character)
{
    if (Character)
    {
        FVector StartLocation = Character->GetActorLocation();
        FVector EndLocation = StartLocation + (Character->GetActorForwardVector() * 200.0f); // Adjust distance as necessary

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Character); // Ignore the player character itself

        // Perform a line trace to check for an enemy in front of the player
        bool bHit = Character->GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            ECC_Visibility,
            QueryParams
        );

        if (bHit && HitResult.GetActor())
        {
            ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor());
            if (HitCharacter)
            {
                switch (AbilityLevel)
                {
                case 1:
                    // Perform the basic three-hit combo
                    UE_LOG(LogTemp, Log, TEXT("Performing Level 1: Basic three-hit combo"));

                    // Insert logic here for basic three-hit combo animations or effects.
                    break;

                case 2:
                    // Perform the combo, adding a fourth hit with knockback
                    UE_LOG(LogTemp, Log, TEXT("Performing Level 2: Four-hit combo with knockback"));

                    // Apply Knockback Effect to the enemy character
                    ApplyKnockbackEffect(HitCharacter);
                    break;

                case 3:
                    // Perform the combo with an energy wave at the end
                    UE_LOG(LogTemp, Log, TEXT("Performing Level 3: Four-hit combo with energy wave"));

                    // Apply Knockback Effect to the enemy character
                    ApplyKnockbackEffect(HitCharacter);

                    // Trigger Energy Wave to affect additional enemies
                    TriggerEnergyWave(Character);
                    break;

                default:
                    UE_LOG(LogTemp, Warning, TEXT("Invalid ability level"));
                    break;
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No enemy detected in front of the character"));
        }
    }
}

/**
 * @brief Applies a knockback effect to the targeted enemy.
 *
 * This function calculates the knockback direction and applies a force to the enemy character to knock them backward.
 *
 * @param HitCharacter The character that will receive the knockback effect.
 */
void UCosmicStrikeAbility::ApplyKnockbackEffect(ACharacter* HitCharacter)
{
    if (HitCharacter)
    {
        FVector KnockbackDirection = HitCharacter->GetActorLocation() - GetOwningActorFromActorInfo()->GetActorLocation();
        KnockbackDirection.Z = 0.0f; // Prevent vertical knockback
        KnockbackDirection.Normalize();

        float KnockbackStrength = 1000.0f; // Adjust based on gameplay balance
        HitCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
        UE_LOG(LogTemp, Log, TEXT("Applying knockback effect to the enemy character."));
    }
}

/**
 * @brief Triggers an energy wave attack.
 *
 * This function is used at the end of the combo to add additional area-of-effect damage.
 *
 * @param Character The character initiating the energy wave attack.
 */
void UCosmicStrikeAbility::TriggerEnergyWave(ACharacter* Character)
{
    if (Character)
    {
        // Logic for triggering energy wave.
        UE_LOG(LogTemp, Log, TEXT("Triggering energy wave."));
    }
}
