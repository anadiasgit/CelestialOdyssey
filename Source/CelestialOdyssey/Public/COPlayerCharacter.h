#pragma once

#include "CoreMinimal.h"
#include "COBaseCharacter.h"
#include "COPlayerCharacter.generated.h"

/**
 *  Player-specific character class.
 *  Inherits basic movement functionality and adds player-specific properties.
 */
class UAbilitySystemComponent;

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

	//Getter functions to aid in animation blueprint
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool GetIsJumpingMoving() const { return bIsJumpingMoving; }

	UFUNCTION(BlueprintPure, Category = "Movement")
	bool GetIsJumpingIdle() const { return bIsJumpingIdle; }

	UFUNCTION(BlueprintPure, Category = "Movement")
	bool GetIsFalling() const { return bIsFalling; }

	// Getter for bHasReachedCeiling
	UFUNCTION(BlueprintCallable, Category = "Gravity Shift")
	bool GetHasReachedCeiling() const { return bHasReachedCeiling; }

	// Setter for bHasReachedCeiling
	void SetHasReachedCeiling(bool bValue) { bHasReachedCeiling = bValue; }

	//This function allows the character to interact with abilities that are owned by the Player State.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

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

	//Keeps track of the direction the character is currently facing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsFacingRight;
	
	//Keeps track of jumping for animation purposes and distinguishes jumping from falling
	bool bIsJumpingMoving = false;
	bool bIsJumpingIdle = false;

	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Gravity Shift")
	bool bHasReachedCeiling = false;

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
