#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrystalShatterAbility.generated.h"

/**
 * @class UCrystalShatterAbility
 * @brief Ability that shatters nearby crystals causing damage and area control.
 *
 * This ability allows players to shatter crystals, creating damaging fragments and
 * leaving slowing fields. Higher levels increase damage, add DoT effects, and improve
 * fragment ricochet capabilities.
 */
UCLASS()
class CELESTIALODYSSEY_API UCrystalShatterAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UCrystalShatterAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

    /** Performs the crystal shatter effect */
    UFUNCTION(BlueprintCallable, Category = "Crystal Shatter")
    void PerformShatter(const FVector& Location);

    /** Creates the slowing field on the ground */
    UFUNCTION(BlueprintCallable, Category = "Crystal Shatter")
    void CreateSlowField(const FVector& Location);

    /** Launches crystal fragments */
    UFUNCTION(BlueprintCallable, Category = "Crystal Shatter")
    void LaunchFragments(const FVector& Origin);

    /** Current level of the ability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Shatter")
    int32 ShatterLevel;

    /** Duration of the slowing field */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Shatter")
    float SlowFieldDuration;

    /** Radius of the shatter effect */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Shatter")
    float ShatterRadius;

    /** Base damage of the shatter */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crystal Shatter")
    float BaseDamage;

    /** Gameplay effect for the cooldown */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> CooldownEffectClass;

    /** Gameplay effect for initial damage */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    /** Gameplay effect for damage over time */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> DoTEffectClass;

    /** Gameplay effect for the slow effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> SlowEffectClass;

private:
    /** Timer handle for the slow field */
    FTimerHandle SlowFieldTimerHandle;
};