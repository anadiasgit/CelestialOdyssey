#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "COPlayerAttributeSet.h"
#include "COGameEnums.h" // Add this new include
#include "COPlayerState.generated.h"

/**
 * @class ACOPlayerState
 * @brief Represents the state of the player, managing persistent gameplay data and abilities.
 *
 * This class is responsible for maintaining the player's state, such as abilities
 * and other data that persists across respawns. Implements the IAbilitySystemInterface for
 * integration with the Gameplay Ability System (GAS).
 */
UCLASS()
class CELESTIALODYSSEY_API ACOPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    /** Default constructor */
    ACOPlayerState();

    /** Current state of the character for input handling */
    UPROPERTY(BlueprintReadWrite, Category = "Input Combo State")
    EInputComboState CurrentInputComboState;

    /** Current level the player is in */
    UPROPERTY(BlueprintReadWrite, Category = "Level")
    ECOGameLevel CurrentLevel;

    /** Mapping of abilities for each level */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
    TMap<ECOGameLevel, FCOLevelAbilityMapping> LevelAbilityMappings;

    /** Gets the Ability System Component */
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    /** Initializes the player's attributes */
    void InitializeAttributes();

    /** Sets the current input combo state and logs any changes */
    void SetCurrentInputComboState(EInputComboState NewState);

    /**
     * @brief Changes the current level and updates available abilities
     * @param NewLevel The level to switch to
     */
    UFUNCTION(BlueprintCallable, Category = "Level Management")
    void SetCurrentLevel(ECOGameLevel NewLevel);

    /**
     * @brief Gets the ability assigned to a specific slot for the current level
     * @param Slot The ability slot to query
     * @return The gameplay ability class assigned to that slot, or nullptr if none
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    TSubclassOf<UGameplayAbility> GetAbilityForSlot(ECOAbilitySlot Slot) const;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> CelestialDashAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> GroundSlamAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> GravityShiftAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> CosmicStrikeAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> CrystalGrowthAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> CrystalShatterAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> VineWhipAbilityClass;

    /** @deprecated Use LevelAbilityMappings instead */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Legacy")
    TSubclassOf<UGameplayAbility> LunarForestFuryAbilityClass;

protected:
    /** Called when the game starts */
    virtual void BeginPlay() override;

    /** Updates available abilities based on current level */
    void UpdateAvailableAbilities();

    /** Ability System Component that manages abilities */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    UAbilitySystemComponent* AbilitySystemComponent;

    /** Attribute Set for player attributes */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    UCOPlayerAttributeSet* AttributeSet;

    /** Data table for initializing attributes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    UDataTable* AttributeDataTable;
};