#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GroundSlamAbility.generated.h"

/**
 * @class UGroundSlamAbility
 * @brief Ability that performs a ground slam to create a shockwave and deal area damage.
 */
UCLASS()
class CELESTIALODYSSEY_API UGroundSlamAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//Default constructor
	UGroundSlamAbility();

protected:
	/** Ground Slam level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Slam Progression")
	int32 GroundSlamLevel;

	/** Ground Slam damage value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Slam Progression")
	float GroundSlamDamage;

	/** Ground Slam radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Slam Progression")
	float GroundSlamRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Slam Effects")
	TSubclassOf<UGameplayEffect> GroundSlamDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Slam Effects")
	TSubclassOf<UGameplayEffect> GroundSlamStunEffect;

public:
	/** Getter for Ground Slam level */
	UFUNCTION(BlueprintCallable, Category="Ground Slam Progression")
	int32 GetGroundSlamLevel() const { return GroundSlamLevel; }

	/** Setter for Ground Slam level */
	UFUNCTION(BlueprintCallable, Category = "Ground Slam Progression")
	void SetGroundSlamLevel(int32 NewLevel) { GroundSlamLevel = NewLevel; }

	/** Getter for Ground Slam damage */
	UFUNCTION(BlueprintCallable, Category = "Ground Slam Progression")
	float GetGroundSlamDamage() const { return GroundSlamDamage; }

	/** Setter for Ground Slam damage */
	UFUNCTION(BlueprintCallable, Category = "Ground Slam Progression")
	void SetGroundSlamDamage(float NewDamage) { GroundSlamDamage = NewDamage; }

	/** Getter for Ground Slam radius */
	UFUNCTION(BlueprintCallable, Category = "Ground Slam Progression")
	float GetGroundSlamRadius() const { return GroundSlamRadius; }

	/** Setter for Ground Slam radius */
	UFUNCTION(BlueprintCallable, Category = "Ground Slam Progression")
	void SetGroundSlamRadius(float NewRadius) { GroundSlamRadius = NewRadius; }

protected:
	// Called when the ability is activated 
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
