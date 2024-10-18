#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "COPlayerAttributeSet.generated.h"

/**
 * @class UCOPlayerAttributeSet
 * @brief Attribute Set that defines the player attributes such as Lives.
 */
UCLASS()
class CELESTIALODYSSEY_API UCOPlayerAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UCOPlayerAttributeSet();

    /** Lives attribute */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Lives;

    /** Getter for Lives attribute */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetLives() const { return Lives.GetCurrentValue(); }

    /** Setter for Lives attribute */
    void SetLives(float NewLives) { Lives.SetCurrentValue(NewLives); }
};