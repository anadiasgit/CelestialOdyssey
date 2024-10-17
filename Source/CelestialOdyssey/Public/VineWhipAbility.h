#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VineWhipAbility.generated.h"

/** Enum to represent different vine actions */
UENUM(BlueprintType)
enum class EVineWhipAction : uint8
{
    Grab      UMETA(DisplayName = "Grab"),
    Swing     UMETA(DisplayName = "Swing"),
    Bridge    UMETA(DisplayName = "Bridge"),
};

/**
 * @class UVineWhipAbility
 * @brief Ability that summons a vine for grabbing, swinging, or bridging.
 *
 * This ability allows the player to grab objects, swing across gaps, or pull enemies closer.
 */
UCLASS()
class CELESTIALODYSSEY_API UVineWhipAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UVineWhipAbility();

    /** Getter for VineWhipLevel */
    int32 GetVineWhipLevel() const { return VineWhipLevel; }

    /** Setter for VineWhipLevel */
    void SetVineWhipLevel(int32 NewLevel) { VineWhipLevel = NewLevel; }

    /** Setter for VineWhipAction */
    void SetVineWhipAction(EVineWhipAction NewAction) { VineWhipAction = NewAction; }

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    /** Current level of the Vine Whip ability */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vine Whip Progression")
    int32 VineWhipLevel;

    /** Duration for which the vine persists */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vine Whip Progression")
    float VineDuration;

    /** Type of vine action the player wants to perform */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vine Whip Action")
    EVineWhipAction VineWhipAction;
	
    void ExecuteVineAction(const FVector& StartLocation, const FVector& EndLocation, int32 Level);
};
