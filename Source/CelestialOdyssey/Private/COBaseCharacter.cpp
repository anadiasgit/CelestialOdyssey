#include "COBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

/*
 * Constructor
 * Sets default properties like movement constraints.
 */
ACOBaseCharacter::ACOBaseCharacter()
{
	//Constrain movement to a 2D plane (XZ axis)
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f)); //Lock to XZ plane

	//Default MoveSpeed that derived classes can override
	MoveSpeed = 400.0f;
}

/*
 * Called when the game starts or when spawned.
 */
void ACOBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/*
 * Called every frame to update the character.
 */
void ACOBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * Handles movement input along the X-axis (right/left).
 * @param Value - The amount to move in the X direction.
 */
void ACOBaseCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value * MoveSpeed);
}