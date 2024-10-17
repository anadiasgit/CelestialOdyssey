#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LunarForestFuryAbility.generated.h"

/**
 * @class ULunarForestFuryAbility
 * @brief Ability that channels the energy of the forest and the moon to create an eruption of roots.
 *
 * This ability damages, knocks back enemies, and creates a temporary barrier.
 */
UCLASS()
class CELESTIALODYSSEY_API ULunarForestFuryAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ULunarForestFuryAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Current level of the Lunar Forest Fury ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lunar Forest Fury Progression")
	int32 FuryLevel;

	/** Duration for which the roots remain active */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lunar Forest Fury Progression")
	float RootDuration;

	void EruptRoots(const FVector& EruptionLocation, int32 Level);
};
