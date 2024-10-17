#include "GroundSlamAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for UGroundSlamAbility */
UGroundSlamAbility::UGroundSlamAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Initialize default values
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
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{
		//Update properties based on level progression
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

		//Create the shockwave effect using a sphere collision
		FVector SlamLocation = Character->GetActorLocation();
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(Character);

		UGameplayStatics::ApplyRadialDamage(
			this,
			GroundSlamDamage,
			SlamLocation,
			GroundSlamRadius,
			nullptr, //Use default damage type or create a custom damage class
			IgnoredActors,
			Character,
			Character->GetController(),
			false //Damage is the same throughout the entire shockwave radius
		);
	}

	//End the ability once it is used
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}