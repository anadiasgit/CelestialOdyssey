#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "COGameEnums.h"
#include "COPlayerController.generated.h"

/**
 * @class ACOPlayerController
 * @brief Player-specific controller class that handles input binding and ability activation.
 *
 * This controller manages both basic movement controls and the ability system,
 * including level-specific abilities and combo inputs for Gravity Shift.
 */
UCLASS()
class CELESTIALODYSSEY_API ACOPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    /** Default constructor */
    ACOPlayerController();

    /**
     * @brief Activates an ability from a specific slot
     * @param Slot The slot containing the ability to activate
     * @param Value The input value from Enhanced Input
     */
    void ActivateAbilityFromSlot(ECOAbilitySlot Slot, const FInputActionValue& Value);
protected:
    /**
     * @brief Wrapper function to activate the Dash ability from the ability slot system
     * @param Value The input value from Enhanced Input
     */
    void ActivateDashAbilityWrapper(const FInputActionValue& Value)
    {
        ActivateAbilityFromSlot(ECOAbilitySlot::DashAbility, Value);
    }

    /**
     * @brief Wrapper function to activate the Basic Attack ability from the ability slot system
     * @param Value The input value from Enhanced Input
     */
    void ActivateBasicAttackWrapper(const FInputActionValue& Value)
    {
        ActivateAbilityFromSlot(ECOAbilitySlot::BasicAttack, Value);
    }

    /** @brief Called when the game starts */
    virtual void BeginPlay() override;

    /** @brief Sets up input bindings */
    virtual void SetupInputComponent() override;

    // Movement Input Handlers
    /** @brief Handles horizontal movement input */
    void MoveRight(const FInputActionValue& Value);

    /** @brief Initiates sprint movement */
    void StartSprint(const FInputActionValue& Value);

    /** @brief Ends sprint movement */
    void StopSprint(const FInputActionValue& Value);

    /** @brief Initiates jump */
    void StartJump(const FInputActionValue& Value);

    /** @brief Ends jump */
    void StopJump(const FInputActionValue& Value);

    /** @brief Initiates crouch */
    void StartCrouch(const FInputActionValue& Value);

    /** @brief Ends crouch */
    void StopCrouch(const FInputActionValue& Value);

    /** @brief Handles ground slam or crouch based on character state */
    void HandleGroundSlamOrCrouch(const FInputActionValue& Value);

    // Ability Input Handlers
    /** @brief Handles primary ability input (Q/L1) */
    void HandlePrimaryAbility(const FInputActionValue& Value);

    /** @brief Handles secondary ability input (R/R1) */
    void HandleSecondaryAbility(const FInputActionValue& Value);

    /** @brief Handles combo ability input (Q+R/L1+R1) */
    void HandleComboAbility(const FInputActionValue& Value);

    // Input State Management
    /** @brief Checks for gravity shift combo activation */
    bool CheckForGravityShift();

    /** @brief Resets shoulder button states */
    void ResetShoulderButtonStates();

    /** @brief Updates input mappings when changing levels */
    void UpdateInputMappingForCurrentLevel();

    /**
     * @brief Processes any pending ability activation
     *
     * This function is called after the combo time window expires to activate
     * any pending single-button ability if no combo was detected.
     */
    void ProcessPendingAbility();

    /**
     * @brief Checks if both buttons were pressed within the combo time window
     *
     * @return true if both buttons were pressed within ComboTimeWindow seconds of each other
     */
    bool CheckForSimultaneousPress();

    // Level Properties
    /** @brief Current level identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 CurrentLevel;

    /** @brief Level constants */
    static constexpr int32 EnchantedMoonLevel = 1;
    static constexpr int32 CrystallineCaveLevel = 2;

    // Input Contexts
    /** @brief General input mapping context */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* GeneralInputMappingContext;

    // Input State Tracking
    /** @brief Tracks left shoulder button state */
    bool bGamepadShoulderLeftPressed = false;

    /** @brief Tracks right shoulder button state */
    bool bGamepadShoulderRightPressed = false;

    // Input Actions
    /** @brief Movement right/left */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveRightAction;

    /** @brief Jump action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    /** @brief Crouch/Ground slam action */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* CrouchOrGroundSlamAction;

    /** @brief Sprint action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* SprintAction;

    /** @brief Dash ability action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* DashAction;

    /** @brief Basic attack action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CosmicStrikeAction;

    /** @brief Left shoulder button action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* GamepadShoulderLeftAction;

    /** @brief Right shoulder button action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* GamepadShoulderRightAction;

    /** Time of last left shoulder button/Q key press */
    float LastShoulderLeftPressTime;

    /** Time of last right shoulder button/R key press */
    float LastShoulderRightPressTime;

    /** Time window in seconds for detecting simultaneous button presses */
    static constexpr float ComboTimeWindow = 0.2f;

    /** Tracks if an ability activation is currently pending */
    bool bIsAbilityActivationPending;

    /** Stores the type of ability pending activation */
    ECOAbilitySlot PendingAbilitySlot;
};