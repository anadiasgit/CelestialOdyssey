#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CosmicStrikeAbility.generated.h"

/**
 * @class UCosmicStrikeAbility
 * @brief Ability that implements a basic melee attack combo with progression.
 *
 * This ability allows the player to perform a combo melee attack with different effects at each level.
 */
UCLASS()
class CELESTIALODYSSEY_API UCosmicStrikeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCosmicStrikeAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Current combo level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo Progression")
	int32 ComboLevel;

	void ExecuteCombo(ACharacter* Character);
};
