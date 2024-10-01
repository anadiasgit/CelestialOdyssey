#include "COPlayerController.h"
#include "GameFramework/Pawn.h"
#include "COPlayerCharacter.h"


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
		//Bind movement
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACOPlayerController::MoveRight);

		//Bind jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACOPlayerController::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopJump);

		//Bind crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACOPlayerController::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopCrouch);

		// Bind sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACOPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACOPlayerController::StopSprint);
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
 * Handles start of player crouch input
 * 
 * @param Value - Unused, but required by the Enhanced Input System. Initiates the crouch action.
 */
void ACOPlayerController::StartCrouch(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ACOPlayerCharacter* PlayerCharacter = Cast <ACOPlayerCharacter>(ControlledPawn))
		{
			PlayerCharacter->StartCrouch();
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
		if (ACOPlayerCharacter* PlayerCharacter = Cast <ACOPlayerCharacter>(ControlledPawn))
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
