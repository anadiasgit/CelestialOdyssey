#include "COPlayerController.h"
#include "GameFramework/Pawn.h"
#include "COPlayerCharacter.h"
#include "COPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


/**
 * Constructor
 * Initializes the player controller.
 */
ACOPlayerController::ACOPlayerController()
{

}

/**
 * Called when the game starts or when spawned
 */
void ACOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Ensure PlayerInputMappingContext is valid
	if (!PlayerInputMappingContext)
	{
		return;
	}

	// Get the local player subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Add the player input mapping context
		Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
	}

}


/**
 * Called to bind input actions
 */
void ACOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind movement
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACOPlayerController::MoveRight);

		// Bind jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACOPlayerController::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopJump);

		// Bind sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACOPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopSprint);

		// Bind shared action for crouch or ground slam
		EnhancedInputComponent->BindAction(CrouchOrGroundSlamAction, ETriggerEvent::Started, this, &ACOPlayerController::HandleGroundSlamOrCrouch);
		EnhancedInputComponent->BindAction(CrouchOrGroundSlamAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopCrouch);

		EnhancedInputComponent->BindAction(GamepadShoulderLeftAction, ETriggerEvent::Started, this, &ACOPlayerController::OnGamepadShoulderLeftPressed);
		EnhancedInputComponent->BindAction(GamepadShoulderLeftAction, ETriggerEvent::Completed, this, &ACOPlayerController::OnGamepadShoulderLeftReleased);

		EnhancedInputComponent->BindAction(GamepadShoulderRightAction, ETriggerEvent::Started, this, &ACOPlayerController::OnGamepadShoulderRightPressed);
		EnhancedInputComponent->BindAction(GamepadShoulderRightAction, ETriggerEvent::Completed, this, &ACOPlayerController::OnGamepadShoulderRightReleased);

		// Bind Abilities
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACOPlayerController::ActivateDashAbility);
		EnhancedInputComponent->BindAction(CosmicStrikeAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateCosmicStrikeAbility);
		EnhancedInputComponent->BindAction(CrystalGrowthAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateCrystalGrowthAbility);
		EnhancedInputComponent->BindAction(CrystalShatterAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateCrystalShatterAbility);
		EnhancedInputComponent->BindAction(VineWhipAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateVineWhipAbility);
		EnhancedInputComponent->BindAction(LunarForestFuryAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateLunarForestFuryAbility);
	}
}

/**
 * Handles player movement input (right/left)
 * 
 * @param Value - The input value representing movement along the X-axis. Positive values move right, negative values move left.
 */
void ACOPlayerController::MoveRight(const FInputActionValue& Value)
{
	//Get the player character and move them along the X-axis
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			float MovementValue = Value.Get<float>();
			PlayerCharacter->MoveRight(MovementValue);
		}
	}
}

/**
 * Handles start of player jump input
 * 
 * @param Value - Unused, but required by the Enhanced Input System. Initiates the jump action.
 */
void ACOPlayerController::StartJump(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast <ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StartJump();
		}
	}
}

/**
 * Handles stop of player jump input
 * 
 * @param Value - Unused, but required by the Enhanced Input System. Ends the jump action.
 */
void ACOPlayerController::StopJump(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast <ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StopJump();
		}
	}
}

/**
 * Handles the input for either Ground Slam or Crouch depending on the player's state.
 *
 * @param Value - Unused, but required by the Enhanced Input System.
 */
void ACOPlayerController::HandleGroundSlamOrCrouch(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			// Check if character is in the air
			if (PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				// If in the air, activate Ground Slam
				ActivateGroundSlamAbility(Value);
			}
			else
			{
				// If on the ground, crouch
				PlayerCharacter->StartCrouch();
			}
		}
	}
}

/**
 * Handles stop of player crouch input
 *
 * @param Value - Unused, but required by the Enhanced Input System. Ends the crouch action.
 */
void ACOPlayerController::StopCrouch(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StopCrouch();
		}
	}
}

/**
 * Handles start of player sprint input
 *
 * @param Value - Unused, but required by the Enhanced Input System. Initiates the sprint action.
 */
void ACOPlayerController::StartSprint(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StartSprint();
		}
	}
}

/**
 * Handles stop of player sprint input
 *
 * @param Value - Unused, but required by the Enhanced Input System. Ends the sprint action.
 */
void ACOPlayerController::StopSprint(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StopSprint();
		}
	}
}

/**
 * @brief Activates the Celestial Dash ability.
 *
 * This function attempts to activate the Celestial Dash ability for the player character
 * by using the ability system component attached to the character.
 *
 * @param Value Unused input value, required by the input binding system.
 */
void ACOPlayerController::ActivateDashAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->CelestialDashAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->CelestialDashAbilityClass);
				}
			}
		}
	}
}

/**
 * @brief Activates the Ground Slam ability.
 *
 * This function attempts to activate the Ground Slam ability for the player character
 * by using the ability system component attached to the character.
 *
 * @param Value Unused input value, required by the input binding system.
 */
void ACOPlayerController::ActivateGroundSlamAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->GroundSlamAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->GroundSlamAbilityClass);
				}
			}
		}
	}
}

/**
 * @brief Handles activation of the Gravity Shift ability.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateGravityShiftAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->GravityShiftAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->GravityShiftAbilityClass);
				}
			}
		}
	}

}/**
 * Handles activation of the Cosmic Strike ability.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateCosmicStrikeAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->CosmicStrikeAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->CosmicStrikeAbilityClass);
				}
			}
		}
	}
}

/**
 * Handles activation of the Crystal Growth ability.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateCrystalGrowthAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->CrystalGrowthAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->CrystalGrowthAbilityClass);
				}
			}
		}
	}
}

/**
 * Handles activation of the Crystal Shatter ability.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateCrystalShatterAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->CrystalShatterAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->CrystalShatterAbilityClass);
				}
			}
		}
	}
}

/**
 * Handles activation of the Vine Whip ability.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateVineWhipAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->VineWhipAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->VineWhipAbilityClass);
				}
			}
		}
	}
}

/**
 * @brief Handles the activation of the Lunar Forest Fury ability.
 *
 * This method allows the player to channel energy, triggering Lunar Forest Fury to damage and knockback enemies.
 *
 * @param Value Unused input value, required by the Enhanced Input System.
 */
void ACOPlayerController::ActivateLunarForestFuryAbility(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				// Get the player state
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState && COPlayerState->LunarForestFuryAbilityClass)
				{
					ASC->TryActivateAbilityByClass(COPlayerState->LunarForestFuryAbilityClass);
				}
			}
		}
	}
}

/**
 * @brief Called when the left gamepad shoulder button is pressed.
 *
 * @param Value The input value representing the left shoulder button.
 */
void ACOPlayerController::OnGamepadShoulderLeftPressed(const FInputActionValue& Value)
{
	bGamepadShoulderLeftPressed = true;
	CheckForGravityShift();
}

/**
 * @brief Called when the left gamepad shoulder button is released.
 *
 * @param Value The input value representing the left shoulder button.
 */
void ACOPlayerController::OnGamepadShoulderLeftReleased(const FInputActionValue& Value)
{
	bGamepadShoulderLeftPressed = false;
}

/**
 * @brief Called when the right gamepad shoulder button is pressed.
 *
 * @param Value The input value representing the right shoulder button.
 */
void ACOPlayerController::OnGamepadShoulderRightPressed(const FInputActionValue& Value)
{
	bGamepadShoulderRightPressed = true;
	CheckForGravityShift();
}

/**
 * @brief Called when the right gamepad shoulder button is released.
 *
 * @param Value The input value representing the right shoulder button.
 */
void ACOPlayerController::OnGamepadShoulderRightReleased(const FInputActionValue& Value)
{
	bGamepadShoulderRightPressed = false;
}

/**
 * @brief Checks if both gamepad shoulder buttons are pressed to activate Gravity Shift.
 */
void ACOPlayerController::CheckForGravityShift()
{
	if (bGamepadShoulderLeftPressed && bGamepadShoulderRightPressed)
	{
		ActivateGravityShiftAbility(FInputActionValue());
	}
}