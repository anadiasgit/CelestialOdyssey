#include "COPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
		//Determine which way the player is facing based on input direction
		bool bIsMovingRight = Value > 0.0f;

		//Flip the character if the input direction has changed
		if (bIsMovingRight != bIsFacingRight)
		{
			bIsFacingRight = bIsMovingRight;

			//Rotate the character's mesh 180 degrees to face the other direction
			FRotator NewRotation = GetMesh()->GetComponentRotation();
			NewRotation.Yaw += 180.0f;
			GetMesh()->SetWorldRotation(NewRotation);
		}
	}

	//Handle crouching and sprinting logic
	if (bIsCrouched)
	{
		//Move slower when crouching
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value * CrouchSpeed);
	}
	else if (bIsSprinting)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value * SprintSpeed);
	}
	else
	{
		Super::MoveRight(Value);
	}
}

/**
 *  Handles player jump start input.
 */
void ACOPlayerCharacter::StartJump()
{
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	ACharacter::Jump();
}

/**
 *  Handles player jump stop input.
 */
void ACOPlayerCharacter::StopJump()
{
	ACharacter::StopJumping();
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