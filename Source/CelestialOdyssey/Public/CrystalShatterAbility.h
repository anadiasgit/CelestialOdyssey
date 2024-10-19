#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrystalShatterAbility.generated.h"

/**
 * @class UCrystalShatterAbility
 * @brief Ability that charges and shatters nearby crystals, causing sharp fragments to fly in all directions.
 *
 * This ability allows the player to shatter crystals with charged blasts, causing area damage and creating glowing shards on the ground.
 */
UCLASS()
class CELESTIALODYSSEY_API UCrystalShatterAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCrystalShatterAbility();

	/** Getter for ShatterLevel */
	int32 GetShatterLevel() const { return ShatterLevel; }

	/** Setter for ShatterLevel */
	void SetShatterLevel(int32 NewLevel) { ShatterLevel = NewLevel; }

protected:
	/** Shatter level progression */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crystal Shatter Progression")
	int32 ShatterLevel;

	/** Duration for which the shards remain on the ground */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crystal Shatter Progression")
	float ShardDuration;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

private:
	/** Cooldown duration for the ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crystal Shatter Progression", meta = (AllowPrivateAccess = "true"))
	float CooldownDuration;
};
