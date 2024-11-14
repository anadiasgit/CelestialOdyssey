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
    ULunarForestFuryAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    /** Current level of the Lunar Forest Fury ability */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lunar Forest Fury Progression")
    int32 FuryLevel;

    /** Duration for which the roots remain active */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lunar Forest Fury Progression")
    float RootDuration;

    /** Gameplay Effect applied when ability is on cooldown */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Effects")
    TSubclassOf<UGameplayEffect> CooldownEffect;

    /** Gameplay Effect for base damage */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Effects")
    TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

    /** Gameplay Effect for rooting enemies (Level 2+) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Effects")
    TSubclassOf<UGameplayEffect> RootGameplayEffectClass;

    /** Gameplay Effect for damage over time (Level 3) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Effects")
    TSubclassOf<UGameplayEffect> DoTGameplayEffectClass;
};