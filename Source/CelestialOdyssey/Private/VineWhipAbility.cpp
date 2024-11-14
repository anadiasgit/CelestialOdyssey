#include "VineWhipAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for UVineWhipAbility */
UVineWhipAbility::UVineWhipAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set default level and vine duration
    VineWhipLevel = 1;
    VineDuration = 8.0f;

    VineWhipAction = EVineWhipAction::Grab; // Default to grab
}

/**
 * @brief Activates the Vine Whip ability.
 *
 * This function allows the player to use a vine for different actions, such as grabbing objects,
 * swinging across gaps, or creating a bridge. The action depends on the player's input and progression level.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UVineWhipAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Get the direction and location to use the vine
        FVector StartLocation = Character->GetActorLocation();
        FVector EndLocation = StartLocation + Character->GetActorForwardVector() * 500.0f;

        // TODO: Replace with Gameplay Targeting to get precise location and target.
        ExecuteVineAction(StartLocation, EndLocation, VineWhipLevel);

        // Set a timer to destroy the vine after the duration ends
        FTimerHandle VineTimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            VineTimerHandle,
            [this, StartLocation]()
        {
            // TODO: Destroy the vine here using proper Gameplay Effect
            UE_LOG(LogTemp, Log, TEXT("Vine at %s destroyed."), *StartLocation.ToString());
        },
            VineDuration,
            false
        );

        // End the ability after activation
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
    }
}

/**
 * @brief Ends the Vine Whip ability and removes relevant gameplay tags.
 *
 * @param Handle Handle that identifies the ability being ended.
 * @param ActorInfo Information about the actor that activated the ability.
 * @param ActivationInfo Information about the context of the ability ending.
 * @param bReplicateEndAbility Should replication of the ability's end occur.
 * @param bWasCancelled Was the ability manually canceled.
 */
void UVineWhipAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }
}

/**
 * @brief Executes the vine action based on the current ability level.
 *
 * This function handles grabbing objects, swinging, or creating vine bridges based on the player's input and ability progression.
 *
 * @param StartLocation The start location of the vine.
 * @param EndLocation The intended end location for the vine action.
 * @param Level The level of the vine whip, defining the possible actions.
 */
void UVineWhipAbility::ExecuteVineAction(const FVector& StartLocation, const FVector& EndLocation, int32 Level)
{
    // TODO: Replace action level logic with Gameplay Tags to manage progression and add targeting to decide action dynamically.
    switch (VineWhipAction)
    {
    case EVineWhipAction::Grab:
        if (Level >= 1)
        {
            UE_LOG(LogTemp, Log, TEXT("Executing grab action from %s to %s"), *StartLocation.ToString(), *EndLocation.ToString());
            // TODO: Implement logic to grab an object or pull an enemy using proper Gameplay Effect
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Grab action not available at level %d"), Level);
        }
        break;

    case EVineWhipAction::Swing:
        if (Level >= 1)
        {
            UE_LOG(LogTemp, Log, TEXT("Swinging from %s to %s"), *StartLocation.ToString(), *EndLocation.ToString());
            // TODO: Implement logic to swing across gaps using proper Gameplay Mechanic
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Swing action not available at level %d"), Level);
        }
        break;

    case EVineWhipAction::Bridge:
        if (Level >= 3)
        {
            UE_LOG(LogTemp, Log, TEXT("Creating vine bridge from %s to %s"), *StartLocation.ToString(), *EndLocation.ToString());
            // TODO: Implement logic to create a bridge between two points using proper Gameplay Mechanic
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Bridge action not available at level %d"), Level);
        }
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown vine whip action"));
        break;
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
bool UVineWhipAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // Check tags in Ability System Component (ASC)
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.VineWhip"))))
        {
            return false;
        }

        // Check if any ability is currently casting
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }

    return true;
}
