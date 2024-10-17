#pragma once

#include "AbilityInputEnum.generated.h"

/**
 * @enum EAbilityInput
 * @brief Enum to manage input bindings for different abilities in the game.
 *
 * This enum is used to assign and identify input actions for the player's abilities,
 * such as the Celestial Dash.
 */
UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
    None        UMETA(DisplayName = "None"),
    CelestialDash UMETA(DisplayName = "Celestial Dash"),
    GroundSlam UMETA(DisplayName = "Ground Slam"),
    GravityShift UMETA(DisplayName = "Gravity Shift"),
    CosmicStrike UMETA(DisplayName = "Cosmic Strike"),
    CrystalGrowth UMETA(DisplayName = "Crystal Growth"),
    CrystalShatter UMETA(DisplayName = "Crystal Shatter"),
    VineWhip UMETA(DisplayName = "Vine Whip"),
    LunarForestFury UMETA(DisplayName = "Lunar Forest Fury"),
};
