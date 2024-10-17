#include "CelestialDashAbility.h"
#include "GameFramework/Character.h"

/** Default constructor for UCelestialDashAbility */
UCelestialDashAbility::UCelestialDashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Initialize the default level of the dash and its speed
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
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	if (Character)
	{
		
		FVector DashDirection = Character->GetActorForwardVector(); // Gets the direction the character is facing.
		float DashDistance;

		switch (DashLevel)
		{
		case 1:
			DashDistance = 400.0f; //Short-range dash for Level 1
			break;
		case 2:
			DashDistance = 600.0f; //Medium-range dash for Level 2
			break;
		case 3:
			DashDistance = 800.0f; //Long-range dash for Level 3
			break;
		default:
			DashDistance = 400.0f; //Default to Level 1
			break;
		}
    
		// Calculate the dash destination
		FVector DashDestination = Character->GetActorLocation() + DashDirection * DashDistance;

		Character->LaunchCharacter(DashDirection * DashSpeed, true, true);
	}

	//End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}