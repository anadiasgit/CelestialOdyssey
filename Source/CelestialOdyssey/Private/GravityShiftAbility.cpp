#include "GravityShiftAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

/** Default constructor for UGravityShiftAbility */
UGravityShiftAbility::UGravityShiftAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Set default level
	GravityShiftLevel = 1;

	//Set the default duration for the gravity inversion
	GravityShiftDuration = 5.0f;
}

/**
 * @brief Activates the gravity shift ability.
 *
 * This function executes the logic for inverting gravity, rotating the player,
 * and allowing them to float upwards. After the specified duration, gravity is reverted back to normal.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UGravityShiftAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		//Set duration based on progression level
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
				GravityShiftDuration = 5.0f; //Default to level 1
				break;
		}

		//Invert gravity by multiplying by -1
		Character->GetCharacterMovement()->GravityScale = -1.0f;

		//Rotate the character to match the inverted gravity
		RotateCharacter(Character, true);

		//Set a timer to revert gravity after GravityShiftDuration
		FTimerHandle TimerHandle;
		Character->GetWorldTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &UGravityShiftAbility::RevertGravity, Character),
			GravityShiftDuration,
			false);

		//End the ability after duration ends
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
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
}

/**
 * @brief Reverts the gravity back to normal.
 *
 * @param Character The character whose gravity needs to be reverted.
 */
void UGravityShiftAbility::RevertGravity(ACharacter* Character)
{
	if (Character)
	{
		//Revert gravity to normal
		Character->GetCharacterMovement()->GravityScale = 1.0f;

		//Rotate the character back to standing upright
		RotateCharacter(Character, false);
	}
}

/**	
 * @brief Rotates the character to align with gravity shift.
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
			//Rotate 180 degrees to make the character's feet point upwards
			NewRotation.Roll = 180.0f;
		}
		else
		{
			//Set back to normal
			NewRotation.Roll = 0.0f;
		}
		Character->SetActorRotation(NewRotation);
	}
}