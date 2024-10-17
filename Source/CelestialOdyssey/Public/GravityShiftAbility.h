#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GravityShiftAbility.generated.h"

/**
 * @class UGravityShiftAbility
 * @brief Ability that inverts gravity for the player temporarily and rotates the character accordingly.
 */
UCLASS()
class CELESTIALODYSSEY_API UGravityShiftAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	/** Default constructor */
	UGravityShiftAbility();

	/** Getter for GravityShiftLevel */
	int32 GetGravityShiftLevel() const { return GravityShiftLevel; }

	/** Setter for GravityShiftLevel */
	void SetGravityShiftLevel(int32 NewLevel) { GravityShiftLevel = NewLevel; }

protected:
	/** Current level of the Gravity Shift ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity Shift Progression")
	int32 GravityShiftLevel;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	/** Duration for the gravity inversion */
	float GravityShiftDuration;

	void RevertGravity(ACharacter* Character);

	void RotateCharacter(ACharacter* Character, bool bIsGravityInverted);
};
