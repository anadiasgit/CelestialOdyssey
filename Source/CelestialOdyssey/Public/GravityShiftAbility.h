#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GravityShiftAbility.generated.h"

UCLASS()
class CELESTIALODYSSEY_API UGravityShiftAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGravityShiftAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;

    void RevertGravity(ACharacter* Character);

    bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags,
        const FGameplayTagContainer* TargetTags,
        FGameplayTagContainer* OptionalRelevantTags
    ) const override;

private:
    void RotateCharacter(ACharacter* Character, bool bIsGravityInverted);
    void CheckForCeilingContact(ACharacter* Character);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Shift", meta = (AllowPrivateAccess = "true"))
    float GravityShiftDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Shift", meta = (AllowPrivateAccess = "true"))
    int32 GravityShiftLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UGameplayEffect> CooldownEffect;

    float DefaultCapsuleRadius;
    float DefaultCapsuleHeight;

    /** Handle for the timer that periodically checks if the character has reached the ceiling. */
    FTimerHandle CeilingCheckTimerHandle;
};