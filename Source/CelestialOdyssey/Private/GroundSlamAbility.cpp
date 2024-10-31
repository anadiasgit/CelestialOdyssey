#include "GroundSlamAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for UGroundSlamAbility */
UGroundSlamAbility::UGroundSlamAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Initialize default values
    GroundSlamLevel = 1;
    GroundSlamDamage = 50.0f;
    GroundSlamRadius = 300.0f;
}

/**
 * @brief Activates the ground slam ability.
 *
 * This function executes the logic for the player's ground slam, creating a shockwave
 * and dealing damage to enemies in a radius.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UGroundSlamAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Update properties based on level progression
        switch (GroundSlamLevel)
        {
        case 1:
            GroundSlamDamage = 50.0f;
            GroundSlamRadius = 300.0f;
            break;
        case 2:
            GroundSlamDamage = 75.0f;
            GroundSlamRadius = 450.0f;
            break;
        case 3:
            GroundSlamDamage = 100.0f;
            GroundSlamRadius = 600.0f;
            break;
        default:
            GroundSlamDamage = 50.0f;
            GroundSlamRadius = 300.0f;
            break;
        }

        // Create the shockwave effect using a sphere collision
        FVector SlamLocation = Character->GetActorLocation();
        TArray<AActor*> IgnoredActors;
        IgnoredActors.Add(Character);

        
        if (GroundSlamDamageEffect)
        {
            FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(GroundSlamDamageEffect, 1.0f);
            UGameplayStatics::ApplyRadialDamage(
                this,
                GroundSlamDamage,
                SlamLocation,
                GroundSlamRadius,
                nullptr, // Use default damage type or create a custom damage class
                IgnoredActors,
                Character,
                Character->GetController(),
                false // Damage is the same throughout the entire shockwave radius
            );
        }

        //Stun effect if level 3
        if (GroundSlamLevel == 3 && GroundSlamStunEffect)
        {
            TArray<FHitResult> HitResults;
            FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GroundSlamRadius);
            if (GetWorld()->SweepMultiByChannel(HitResults, SlamLocation, SlamLocation, FQuat::Identity, ECC_Pawn, CollisionShape))
            {
                for (const FHitResult& Hit : HitResults)
                {
                    AActor* HitActor = Hit.GetActor();
                    if (HitActor && HitActor != Character)
                    {
                        if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
                        {
                            // Apply the Ground Slam stun effect
                            FGameplayEffectSpecHandle StunSpecHandle = MakeOutgoingGameplayEffectSpec(GroundSlamStunEffect, 1.0f);
                            FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToTarget(*StunSpecHandle.Data.Get(), TargetASC);

                            // Manually add the State.CC.Stunned tag to the target
                            TargetASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.CC.Stunned")));

                            // Set a timer to remove the tag after the stun effect duration ends
                            float EffectDuration = 2.0f; // Assuming 2 seconds as the stun duration
                            FTimerHandle StunEffectTimerHandle;

                            FTimerDelegate TimerCallback;
                            TimerCallback.BindLambda([TargetASC]()
                            {
                                if (TargetASC)
                                {
                                    TargetASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.CC.Stunned")));
                                }
                            });

                            // Use the World Timer Manager to set the timer with the specified duration
                            GetWorld()->GetTimerManager().SetTimer(StunEffectTimerHandle, TimerCallback, EffectDuration, false);
                        }
                    }
                }
            }
        }


        // Destroy Breakable Objects if Level 3
        if (GroundSlamLevel == 3)
        {
            TArray<FHitResult> HitResults;
            FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GroundSlamRadius);
            if (GetWorld()->SweepMultiByChannel(HitResults, SlamLocation, SlamLocation, FQuat::Identity, ECC_WorldDynamic, CollisionShape))
            {
                for (const FHitResult& Hit : HitResults)
                {
                    AActor* HitActor = Hit.GetActor();
                    if (HitActor && HitActor->ActorHasTag(FName("Environment.Breakable")))
                    {
                        HitActor->Destroy();
                    }
                }
            }
        }
    }

    // End the ability once it is used
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

/**
 * @brief Ends the ground slam ability and removes relevant gameplay tags.
 *
 * @param Handle Handle that identifies the ability being ended.
 * @param ActorInfo Information about the actor that activated the ability.
 * @param ActivationInfo Information about the context of the ability ending.
 * @param bReplicateEndAbility Should replication of the ability's end occur.
 * @param bWasCancelled Was the ability manually canceled.
 */
void UGroundSlamAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
bool UGroundSlamAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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
            // Prevent ability activation if on cooldown or casting another ability
            return false;
        }
    }

    // Check if the character is in the air
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        if (!Character->GetCharacterMovement()->IsFalling())
        {
            // Ability can only be activated if the character is in the air (falling)
            return false;
        }
    }

    return true;
}
