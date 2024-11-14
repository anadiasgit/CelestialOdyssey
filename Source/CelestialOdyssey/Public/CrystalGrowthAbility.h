#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrystalGrowthAbility.generated.h"

UENUM(BlueprintType)
enum class ECrystalStructureType : uint8
{
    Bridge      UMETA(DisplayName = "Bridge"),
    Wall        UMETA(DisplayName = "Wall"),
    Platform    UMETA(DisplayName = "Platform"),
    Staircase   UMETA(DisplayName = "Staircase"),
    Encasement  UMETA(DisplayName = "Encasement"),
};

/**
 * @class UCrystalGrowthAbility
 * @brief Ability that allows players to grow crystal formations for platforming and combat.
 *
 * This ability enables the player to create various crystal structures like bridges, walls,
 * and platforms. At higher levels, it can create more complex formations and even encase enemies.
 */
UCLASS()
class CELESTIALODYSSEY_API UCrystalGrowthAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UCrystalGrowthAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

    /** Spawns the selected crystal structure at the target location */
    UFUNCTION(BlueprintCallable, Category = "Crystal Growth")
    void SpawnCrystalStructure(const FVector& Location, const FRotator& Rotation);

    /** Handles the targeting for crystal growth */
    UFUNCTION(BlueprintCallable, Category = "Crystal Growth")
    FVector GetTargetLocation(ACharacter* Character) const;

    /** Duration for which crystals persist */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Growth")
    float CrystalDuration;

    /** Current level of the ability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Growth")
    int32 GrowthLevel;

    /** Maximum range for crystal growth */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Growth")
    float MaxRange;

    /** Type of crystal structure to create */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Growth")
    ECrystalStructureType StructureType;

    /** Cooldown gameplay effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> CooldownEffectClass;

private:
    /** Timer handle for crystal duration */
    FTimerHandle CrystalTimerHandle;
};