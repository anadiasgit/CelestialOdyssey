#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "COPlayerController.generated.h"

/**
 * Player-specific controller class.
 * Handles input binding and controls the player's character.
 */

UCLASS()
class CELESTIALODYSSEY_API ACOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Constructor
	ACOPlayerController();

protected:
	//Called when the game starts
	virtual void BeginPlay() override;

	//Called to bind input actions
	virtual void SetupInputComponent() override;

	//Input functions for movement, jumping, and crouching
	void MoveRight(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void ActivateDashAbility(const FInputActionValue& Value);
	void ActivateGroundSlamAbility(const FInputActionValue& Value);
	void ActivateGravityShiftAbility(const FInputActionValue& Value);
	void ActivateCosmicStrikeAbility(const FInputActionValue& Value);
	void ActivateCrystalGrowthAbility(const FInputActionValue& Value);
	void ActivateCrystalShatterAbility(const FInputActionValue& Value);
	void ActivateVineWhipAbility(const FInputActionValue& Value);
	void ActivateLunarForestFuryAbility(const FInputActionValue& Value);

	//Input Mapping Context for Player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerInputMappingContext;

	//Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GroundSlamAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GravityShiftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CosmicStrikeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrystalGrowthAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrystalShatterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* VineWhipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LunarForestFuryAction;

};
