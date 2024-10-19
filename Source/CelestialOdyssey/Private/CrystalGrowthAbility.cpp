#include "CrystalGrowthAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for UCrystalGrowthAbility */
UCrystalGrowthAbility::UCrystalGrowthAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set default level and crystal duration
    GrowthLevel = 1;
    CrystalDuration = 15.0f;

    StructureType = ECrystalStructureType::Bridge; // Default to bridge
}

/**
 * @brief Activates the Crystal Growth ability.
 *
 * This function allows the player to create crystal formations in the aimed direction.
 * Depending on the growth level, different types of formations can be generated.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UCrystalGrowthAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 200.0f;

        if (GrowthLevel == 3)
        {
            // TODO: Replace with Gameplay Targeting and Effect to encase enemies in crystals if targeted.
            FHitResult HitResult;
            FVector Start = Character->GetActorLocation();
            FVector End = Start + Character->GetActorForwardVector() * 500.0f;

            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(Character);
            bool bHit = Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

            if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(FName("Enemy")))
            {
                // TODO: Replace manual encasement with a Gameplay Effect for consistency with GAS.
                AActor* HitActor = HitResult.GetActor();
                UE_LOG(LogTemp, Log, TEXT("Encasing enemy in crystal at %s"), *HitResult.Location.ToString());
                EncaseEnemy(HitActor);
            }
            else
            {
                // If no target is found or the target is not an enemy, spawn the selected structure
                SpawnCrystalFormation(SpawnLocation, GrowthLevel);
            }
        }
        else
        {
            // If GrowthLevel is 1 or 2, directly spawn the selected structure
            SpawnCrystalFormation(SpawnLocation, GrowthLevel);
        }

        // Set a timer to destroy the crystal after the duration ends
        FTimerHandle CrystalTimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            CrystalTimerHandle,
            [this, SpawnLocation]()
        {
            // TODO: Destroy the crystal formation here
            UE_LOG(LogTemp, Log, TEXT("Crystal formation at %s destroyed."), *SpawnLocation.ToString());
        },
            CrystalDuration,
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
void UCrystalGrowthAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
bool UCrystalGrowthAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
