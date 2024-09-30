#pragma once

#include "CoreMinimal.h"
#include "COBaseCharacter.h"
#include "COPlayerCharacter.generated.h"

/**
 *  Player-specific character class.
 *  Inherits basic movement functionality and adds player-specific properties.
 */
UCLASS()
class CELESTIALODYSSEY_API ACOPlayerCharacter : public ACOBaseCharacter
{
	GENERATED_BODY()
	
public:
	//Constructor
	ACOPlayerCharacter();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Handles player movement input (right/left)
	virtual void MoveRight(float Value) override;

	//Handles player jump input
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void StartJump();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void StopJump();

	//Handles player crouch input
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void StartCrouch();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void StopCrouch();

	//Handles player sprint input
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopSprint();

protected:
	//Player's default jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float JumpHeight;

	//Player's crouch speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float CrouchSpeed;

	//Player's sprint speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	bool bIsSprinting;

	// Spring Arm for camera follow
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// Adjustable camera arm length
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraArmLength;
};
