#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CosmicStrikeAbility.generated.h"

/**
 * @class UCosmicStrikeAbility
 * @brief Handles the Cosmic Strike ability, which allows the player to perform a melee combo attack.
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

    bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,  const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

    void ApplyKnockbackEffect(ACharacter* HitCharacter);
    void TriggerEnergyWave(ACharacter* Character);

private:
    void PerformAttack(ACharacter* Character);
    UFUNCTION()
    void EndComboAttack();

    /** Current level of Cosmic Strike. Determines the power of the attack. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmic Strike", meta = (AllowPrivateAccess = "true"))
    int32 AbilityLevel;

    /** Cooldown duration for Cosmic Strike. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmic Strike", meta = (AllowPrivateAccess = "true"))
    float AttackCooldown;

    /** Handle for managing combo cooldown reset. */
    FTimerHandle ComboResetTimerHandle;
};
