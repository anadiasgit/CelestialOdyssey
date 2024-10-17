#include "LunarForestFuryAbility.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for ULunarForestFuryAbility */
ULunarForestFuryAbility::ULunarForestFuryAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Set defaults
	FuryLevel = 1;
	RootDuration = 8.0f;
}

/**
 * @brief Activates the Lunar Forest Fury ability.
 *
 * This function channels energy to create a burst of roots that damage, knock back enemies,
 * and create a temporary barrier. Depending on the level, the effects vary.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void ULunarForestFuryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		FVector EruptionLocation = Character->GetActorLocation();

		//TODO: Replace eruption logic with Gameplay Tags and Effects
		EruptRoots(EruptionLocation, FuryLevel);

		// Set a timer to end the root effect after the duration ends
		FTimerHandle RootTimerHandle;
		Character->GetWorldTimerManager().SetTimer(
			RootTimerHandle,
			[this, EruptionLocation]()
		{
			// TODO: Destroy the root barrier and stop any ongoing effects here
			UE_LOG(LogTemp, Log, TEXT("Roots at %s have dissipated."), *EruptionLocation.ToString());
		},
			RootDuration,
			false);

		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

/**
 * @brief Erupts roots at the specified location.
 *
 * Depending on the ability level, the roots damage, knock back enemies, entrap them,
 * or deal damage over time.
 *
 * @param EruptionLocation The location where the roots will erupt.
 * @param Level The level of the Lunar Forest Fury ability.
 */
void ULunarForestFuryAbility::EruptRoots(const FVector& EruptionLocation, int32 Level)
{
	// TODO: Replace eruption logic level with Gameplay Effects
	switch (Level)
	{
	case 1:
		UE_LOG(LogTemp, Log, TEXT("Erupting roots at %s for Level 1"), *EruptionLocation.ToString());
		// TODO: Implement logic to damage and knock back enemies in a small radius
		break;

	case 2:
		UE_LOG(LogTemp, Log, TEXT("Erupting roots at %s for Level 2"), *EruptionLocation.ToString());
		// TODO: Implement increased radius, roots entrap enemies
		break;

	case 3:
		UE_LOG(LogTemp, Log, TEXT("Erupting roots at %s for Level 3"), *EruptionLocation.ToString());
		// TODO: Implement thorns with moonlight that deal damage over time
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown Lunar Forest Fury level"));
		break;
	}
}