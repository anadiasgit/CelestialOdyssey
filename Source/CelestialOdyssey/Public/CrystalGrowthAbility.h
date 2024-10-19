#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrystalGrowthAbility.generated.h"

/** Enum to represent different crystal structures */
UENUM(BlueprintType)
enum class ECrystalStructureType : uint8
{
	Bridge      UMETA(DisplayName = "Bridge"),
	Wall        UMETA(DisplayName = "Wall"),
	Staircase   UMETA(DisplayName = "Staircase"),
};

/**
 * @class UCrystalGrowthAbility
 * @brief Ability that rapidly grows crystal formations in the aimed direction.
 *
 * This ability allows the player to create bridges, walls, or platforms out of crystal.
 */
UCLASS()
class CELESTIALODYSSEY_API UCrystalGrowthAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCrystalGrowthAbility();

	/** Getter for GrowthLevel */
	int32 GetGrowthLevel() const { return GrowthLevel; }

	/** Setter for GrowthLevel */
	void SetGrowthLevel(int32 NewLevel) { GrowthLevel = NewLevel; }

	/** Setter for Crystal Structure Type */
	void SetStructureType(ECrystalStructureType NewType) { StructureType = NewType; }

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Current level of the Crystal Growth ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crystal Growth Progression")
	int32 GrowthLevel;

	/** Duration for which the crystals persist */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crystal Growth Progression")
	float CrystalDuration;

	/** Type of crystal structure the player wants to create */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Growth Type")
	ECrystalStructureType StructureType;

	void SpawnCrystalFormation(const FVector& SpawnLocation, int32 Level);

	void EncaseEnemy(AActor* Target);
};
