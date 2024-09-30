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
};
