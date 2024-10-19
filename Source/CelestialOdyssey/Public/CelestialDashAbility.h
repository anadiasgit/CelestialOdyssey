#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CelestialDashAbility.generated.h"

/**
 * @class UCelestialDashAbility
 * @brief Ability that implements a quick dash movement.
 *
 * This class is responsible for handling a dash ability in the game,
 * allowing the player to quickly move forward. It inherits from UGameplayAbility.
 */
UCLASS()
class CELESTIALODYSSEY_API UCelestialDashAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	//default constructor
	UCelestialDashAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Progression")
	int32 DashLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Progression")
	float DashSpeed;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
