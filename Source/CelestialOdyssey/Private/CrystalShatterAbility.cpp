#include "CrystalShatterAbility.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

/** Default constructor for UCrystalShatterAbility */
UCrystalShatterAbility::UCrystalShatterAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Set defaults
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
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // TODO: Implement Gameplay Effect for cooldown (currently handled manually)

        // Get the character's location for shattering crystals
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

        // End the ability after activation
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
    }
}