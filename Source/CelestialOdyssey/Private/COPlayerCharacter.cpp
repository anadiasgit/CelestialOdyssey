#include "COPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "COPlayerState.h"

/**
 *  Constructor
 *  Sets default properties for the player
 */
ACOPlayerCharacter::ACOPlayerCharacter()
{
	MoveSpeed = 600.0f;
	JumpHeight = 420.0f;
	CrouchSpeed = 300.0f;
	SprintSpeed = 1000.0f;
	bIsSprinting = false;
	bIsFacingRight = true;
	bHasReachedCeiling = false;

	// Create the spring arm component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = CameraArmLength;
	CameraBoom->bUsePawnControlRotation = false; 

	// Create the camera component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

/**
 *  Called when the game starts or when spawned
 */
void ACOPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 *  Called every frame to update the player character.
 */
void ACOPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
	{
		if (!bIsJumpingMoving && !bIsJumpingIdle)
		{
			// Character is falling without a jump
			bIsFalling = true;
		}
	}
	else
	{
		// Character is on the ground
		bIsFalling = false;
		bIsJumpingMoving = false;
		bIsJumpingIdle = false;
	}
}

/**
 *  Handles player movement along the X-axis(right/left).
 *  
 *  @param Value - The input value for movement
 */
void ACOPlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		bool bIsMovingRight = Value > 0.0f;

		// Only trigger turning when the input direction has changed
		if (bIsMovingRight != bIsFacingRight)
		{
			bIsFacingRight = bIsMovingRight;

			// Rotate the character's mesh 180 degrees to face the other direction
			FRotator NewRotation = GetMesh()->GetComponentRotation();
			NewRotation.Yaw += 180.0f;
			GetMesh()->SetWorldRotation(NewRotation);
		}
	}

	// handle crouching and sprinting
	if (bIsCrouched)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value * CrouchSpeed);
	}
	else if (bIsSprinting)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value * SprintSpeed);
	}
	else
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}


/**
 *  Handles player jump start input.
 */
void ACOPlayerCharacter::StartJump()
{
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	ACharacter::Jump();
	// Update booleans based on movement
	if (GetCharacterMovement()->Velocity.Size() > 0.0f)
	{
		bIsJumpingMoving = true;
		bIsJumpingIdle = false;
	}
	else
	{
		bIsJumpingMoving = false;
		bIsJumpingIdle = true;
	}

	bIsFalling = false;
}

/**
 *  Handles player jump stop input.
 */
void ACOPlayerCharacter::StopJump()
{
	ACharacter::StopJumping();
	bIsJumpingMoving = false;
	bIsJumpingIdle = false;
}

/**
 *  Handles crouch input. Sets the player into crouch mode.
 */
void ACOPlayerCharacter::StartCrouch()
{
	ACharacter::Crouch();
}

/**
 *  Handles stop crouch input. Returns the player to normal speed and standing.
 */
void ACOPlayerCharacter::StopCrouch()
{
	ACharacter::UnCrouch();
}

/**
 *  Handles sprint input. Sets the player sprinting.
 */
void ACOPlayerCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;  
}

/**
 *  Handles stop sprint input. Returns the player to normal speed.
 */
void ACOPlayerCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

/**
 * @brief Gets the Ability System Component associated with the Player State.
 *
 * This function allows the character to interact with abilities that are owned by the Player State.
 *
 * @return Pointer to the UAbilitySystemComponent.
 */
UAbilitySystemComponent* ACOPlayerCharacter::GetAbilitySystemComponent() const
{
	ACOPlayerState* COPlayerState = GetPlayerState<ACOPlayerState>();
	return COPlayerState ? COPlayerState->GetAbilitySystemComponent() : nullptr;
}