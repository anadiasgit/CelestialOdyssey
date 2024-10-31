#include "CelestialDashAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "COEnemyAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"


/** Default constructor for UCelestialDashAbility */
UCelestialDashAbility::UCelestialDashAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Initialize the default level of the dash and its speed
    DashLevel = 1;
    DashSpeed = 1500.0f;
}

/**
 * @brief Activates the dash ability.
 *
 * This function executes the logic for the player's dash, which involves moving
 * the character quickly in a specific direction. After the dash is complete,
 * the ability ends.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UCelestialDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

    if (ASC)
    {
        //Apply cooldown effect
        if (SkillCooldownGameplayEffectClass)
        {
            FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(SkillCooldownGameplayEffectClass, 1.0f);
            ASC->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
        }

        // Add "Casting" tag to the player to indicate the ability is in use
        if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.OnCooldown"))))
        {
            ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        }

    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

    if (Character)
    {
        FVector DashDirection = Character->GetActorForwardVector(); // Gets the direction the character is facing.
        float DashDistance;

        switch (DashLevel)
        {
        case 1:
            DashDistance = 400.0f; // Short-range dash for Level 1
            break;
        case 2:
            DashDistance = 600.0f; // Medium-range dash for Level 2
            break;
        case 3:
            DashDistance = 800.0f; // Long-range dash for Level 3
            break;
        default:
            DashDistance = 400.0f; // Default to Level 1
            break;
        }

        // Calculate the dash destination
        FVector DashDestination = Character->GetActorLocation() + DashDirection * DashDistance;

        Character->LaunchCharacter(DashDirection * DashSpeed, true, true);

        // Set up collision detection (dash will be interrupted by collisions)
        FCollisionShape CollisionShape = FCollisionShape::MakeSphere(100.0f); // Radius of 100 for detecting overlaps
        TArray<FHitResult> HitResults;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Character); // Ignore the player character

        if (GetWorld()->SweepMultiByChannel(HitResults, Character->GetActorLocation(), DashDestination, FQuat::Identity, ECC_PhysicsBody, CollisionShape, Params))
        {
            for (const FHitResult& Hit : HitResults)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor && DashLevel == 3 && DamageGameplayEffectClass)
                {
                    if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
                    {
                        FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffectClass, 1.0f);
                        ASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
                    }

                    // Interrupt the dash if a collision occurs
                    Character->GetCharacterMovement()->StopMovementImmediately();
                    break;
                }
            }
        }

        // Maybe use a delay or Ability Task to end the ability properly
    }

    // End the ability after activation, make sure the dash completes before ending
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

/**
 * @brief Checks if the ability can be activated.
 *
 * This function checks if the conditions to activate this ability are met, including whether the character has any tags that would prevent activation (such as being on cooldown).
 *
 * @param Handle - The handle to this specific instance of the ability.
 * @param ActorInfo - Information about the actor using the ability.
 * @param SourceTags - Tags on the source (optional).
 * @param TargetTags - Tags on the target (optional).
 * @param OptionalRelevantTags - Optional tags that may affect ability activation.
 * @return true if the ability can be activated, false otherwise.
 */
bool UCelestialDashAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false; // If base class conditions aren't met, can't activate
    }

    // Check if the character has any tags that prevent activation
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        // Prevent ability activation during cooldown
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.OnCooldown"))))
        {
            return false;
        }

        // Prevent ability activation if another ability is being cast
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Ends the ability and cleans up.
 *
 * @param Handle - The handle to this specific instance of the ability.
 * @param ActorInfo - Information about the actor using the ability.
 * @param ActivationInfo - Additional info about the activation.
 * @param bReplicateEndAbility - Whether to replicate the end ability event.
 * @param bWasCancelled - Whether the ability was cancelled.
 */
void UCelestialDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }
}
