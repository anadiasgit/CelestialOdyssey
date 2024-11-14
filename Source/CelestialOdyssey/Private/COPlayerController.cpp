#include "COPlayerController.h"
#include "GameFramework/Pawn.h"
#include "COPlayerCharacter.h"
#include "COPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


/**
 * @brief Default constructor for ACOPlayerController
 *
 * Initializes timing variables for ability activation tracking.
 */
ACOPlayerController::ACOPlayerController()
{
	LastShoulderLeftPressTime = 0.0f;
	LastShoulderRightPressTime = 0.0f;
	bIsAbilityActivationPending = false;
}

/**
 * Called when the game starts or when spawned
 */
void ACOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GeneralInputMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("GeneralInputMappingContext is null!"));
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Log, TEXT("Adding Input Mapping Context"));
		Subsystem->AddMappingContext(GeneralInputMappingContext, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Local Player Subsystem not found!"));
	}
}

/**
 * @brief Updates input mapping based on the current level.
 */
void ACOPlayerController::UpdateInputMappingForCurrentLevel()
{
	if (ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>())
	{
		COPlayerState->SetCurrentLevel(CurrentLevel == EnchantedMoonLevel ? ECOGameLevel::EnchantedForestMoon : ECOGameLevel::CrystallineCaves);
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
		// Movement bindings
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACOPlayerController::MoveRight);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACOPlayerController::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopJump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACOPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopSprint);
		EnhancedInputComponent->BindAction(CrouchOrGroundSlamAction, ETriggerEvent::Started, this, &ACOPlayerController::HandleGroundSlamOrCrouch);
		EnhancedInputComponent->BindAction(CrouchOrGroundSlamAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopCrouch);

		// Elemental ability bindings
		EnhancedInputComponent->BindAction(GamepadShoulderLeftAction, ETriggerEvent::Started, this, &ACOPlayerController::HandlePrimaryAbility);
		EnhancedInputComponent->BindAction(GamepadShoulderRightAction, ETriggerEvent::Started, this, &ACOPlayerController::HandleSecondaryAbility);

		// Core abilities bindings
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACOPlayerController::ActivateDashAbilityWrapper);
		EnhancedInputComponent->BindAction(CosmicStrikeAction, ETriggerEvent::Started, this, &ACOPlayerController::ActivateBasicAttackWrapper);
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
				// If in the air, activate Ground Slam using slot system
				ActivateAbilityFromSlot(ECOAbilitySlot::GroundSlamAbility, Value);
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
 * @brief Checks if both gamepad shoulder buttons are pressed to activate Gravity Shift.
 * @return true if Gravity Shift was activated, false otherwise
 */
bool ACOPlayerController::CheckForGravityShift()
{
	if (bGamepadShoulderLeftPressed && bGamepadShoulderRightPressed)
	{
		// Activate Gravity Shift using the ability slot system
		ActivateAbilityFromSlot(ECOAbilitySlot::ComboAbility, FInputActionValue());

		// Reset both button states
		ResetShoulderButtonStates();
		return true;
	}
	return false;
}

/**
 * @brief Resets all button states and pending ability flags
 *
 * Called after ability activation to clean up all state tracking variables.
 */
void ACOPlayerController::ResetShoulderButtonStates()
{
	bGamepadShoulderLeftPressed = false;
	bGamepadShoulderRightPressed = false;
	bIsAbilityActivationPending = false;
}

/**
 * @brief Activates an ability from a specific slot
 * @param Slot The slot containing the ability to activate
 * @param Value The input value from Enhanced Input
 */
void ACOPlayerController::ActivateAbilityFromSlot(ECOAbilitySlot Slot, const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(ControlledPawn))
		{
			if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
			{
				ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
				if (COPlayerState)
				{
					// Get the ability for this slot 
					if (TSubclassOf<UGameplayAbility> AbilityClass = COPlayerState->GetAbilityForSlot(Slot))
					{
						ASC->TryActivateAbilityByClass(AbilityClass);
						UE_LOG(LogTemp, Log, TEXT("Activating ability from slot: %d"), static_cast<int32>(Slot));
					}
				}
			}
		}
	}
}

/**
 * @brief Handles the primary ability input (Q/L1)
 *
 * Processes primary ability (Vine Whip) input and checks for potential
 * combo activation with secondary ability input. If both inputs occur within
 * the combo window, activates Gravity Shift instead.
 *
 * @param Value The input value from Enhanced Input
 */
void ACOPlayerController::HandlePrimaryAbility(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	bGamepadShoulderLeftPressed = true;
	LastShoulderLeftPressTime = CurrentTime;

	// Check for simultaneous press first
	if (CheckForSimultaneousPress())
	{
		// Activate Gravity Shift
		ActivateAbilityFromSlot(ECOAbilitySlot::ComboAbility, Value);
		ResetShoulderButtonStates();
	}
	else
	{
		// Store the primary ability as pending
		bIsAbilityActivationPending = true;
		PendingAbilitySlot = ECOAbilitySlot::PrimaryAbility;

		// Set a timer to activate the ability if no combo is detected
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ACOPlayerController::ProcessPendingAbility,
			ComboTimeWindow,
			false
		);
	}
}

/**
 * @brief Handles the secondary ability input (R/R1)
 *
 * Processes secondary ability (Lunar Forest Fury) input and checks for potential
 * combo activation with primary ability input. If both inputs occur within
 * the combo window, activates Gravity Shift instead.
 *
 * @param Value The input value from Enhanced Input
 */
void ACOPlayerController::HandleSecondaryAbility(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	bGamepadShoulderRightPressed = true;
	LastShoulderRightPressTime = CurrentTime;

	// Check for simultaneous press first
	if (CheckForSimultaneousPress())
	{
		// Activate Gravity Shift
		ActivateAbilityFromSlot(ECOAbilitySlot::ComboAbility, Value);
		ResetShoulderButtonStates();
	}
	else
	{
		// Store the secondary ability as pending
		bIsAbilityActivationPending = true;
		PendingAbilitySlot = ECOAbilitySlot::SecondaryAbility;

		// Set a timer to activate the ability if no combo is detected
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ACOPlayerController::ProcessPendingAbility,
			ComboTimeWindow,
			false
		);
	}
}

/**
 * @brief Handles the combo ability input (Q+R/L1+R1)
 */
void ACOPlayerController::HandleComboAbility(const FInputActionValue& Value)
{
	ActivateAbilityFromSlot(ECOAbilitySlot::ComboAbility, Value);
}

/**
 * @brief Checks if both ability buttons were pressed simultaneously
 *
 * Determines if both the primary and secondary ability buttons were pressed
 * within the defined combo time window (ComboTimeWindow seconds).
 *
 * @return true if both buttons were pressed within the time window, false otherwise
 */
bool ACOPlayerController::CheckForSimultaneousPress()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if both buttons are pressed within the combo time window
	if (bGamepadShoulderLeftPressed && bGamepadShoulderRightPressed)
	{
		float TimeDifference = FMath::Abs(LastShoulderLeftPressTime - LastShoulderRightPressTime);
		return TimeDifference <= ComboTimeWindow;
	}

	return false;
}

/**
 * @brief Processes any pending ability activation
 *
 * Called after the combo time window expires. If an ability is pending
 * and no combo was detected, activates the pending ability.
 */
void ACOPlayerController::ProcessPendingAbility()
{
	if (bIsAbilityActivationPending)
	{
		// Only activate the pending ability if a combo was not triggered
		if (!CheckForSimultaneousPress())
		{
			ActivateAbilityFromSlot(PendingAbilitySlot, FInputActionValue());
		}

		bIsAbilityActivationPending = false;
	}
}