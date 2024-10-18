#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "COPlayerAttributeSet.h"
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
	//Default constructor
	ACOPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitializeAttributes();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> CelestialDashAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> GroundSlamAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> GravityShiftAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> CosmicStrikeAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> CrystalGrowthAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> CrystalShatterAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> VineWhipAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayAbility> LunarForestFuryAbilityClass;

protected:
	virtual void BeginPlay() override;

protected:
	//Ability System Component that manages abilities for this player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	/** Attribute Set for player attributes */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UCOPlayerAttributeSet* AttributeSet;

	/** Data table for initializing attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	UDataTable* AttributeDataTable;
};
