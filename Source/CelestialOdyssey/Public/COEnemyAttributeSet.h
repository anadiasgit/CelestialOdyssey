// COEnemyAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "COEnemyAttributeSet.generated.h"

/**
 * @brief A Gameplay Attribute Set used for managing enemy attributes.
 *
 * This class defines attributes for enemies in the game, specifically the health of enemies.
 * It allows for interaction and control over how enemy attributes are manipulated through
 * Gameplay Effects within the Unreal Gameplay Ability System (GAS).
 */
UCLASS()
class CELESTIALODYSSEY_API UCOEnemyAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    /** Default constructor */
    UCOEnemyAttributeSet();

    /** Health attribute for enemies */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Health;

    /** Getter for the Health attribute */
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCOEnemyAttributeSet, Health)
    GAMEPLAYATTRIBUTE_VALUE_GETTER(Health)
    GAMEPLAYATTRIBUTE_VALUE_SETTER(Health)
    GAMEPLAYATTRIBUTE_VALUE_INITTER(Health)

    /** Movement Speed attribute for enemies */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MovementSpeed;

    /** Getter for the Movement Speed attribute */
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCOEnemyAttributeSet, MovementSpeed)
    GAMEPLAYATTRIBUTE_VALUE_GETTER(MovementSpeed)
    GAMEPLAYATTRIBUTE_VALUE_SETTER(MovementSpeed)
    GAMEPLAYATTRIBUTE_VALUE_INITTER(MovementSpeed)

protected:
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
