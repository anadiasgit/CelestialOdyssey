// COEnemyAttributeSet.cpp

#include "COEnemyAttributeSet.h"
#include "GameplayEffectExtension.h"

/** Default constructor */
UCOEnemyAttributeSet::UCOEnemyAttributeSet()
    : Health(100.0f) // Default health value for enemies
{
}

/**
 * @brief Handles attribute changes before they are applied.
 *
 * This function is called before an attribute's value is changed. It ensures that
 * certain attributes, such as Health, remain within valid ranges. In this case,
 * it ensures that the health value does not go below zero.
 *
 * @param Attribute The attribute that is being changed.
 * @param NewValue The new value that is being applied to the attribute.
 */
void UCOEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // Call the base class implementation to ensure default handling
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        // Ensuring health does not go below zero (clamp to a valid value)
        NewValue = FMath::Clamp(NewValue, 0.0f, NewValue);
    }

    if (Attribute == GetMovementSpeedAttribute())
    {
        // Enforce minimum movement speed (e.g., cannot go below zero)
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_FLT);
    }
}
