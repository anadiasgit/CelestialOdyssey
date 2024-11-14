// COGameEnums.h
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "COGameEnums.generated.h"

/**
 * @brief Contains enums and structs for ability and level management in Celestial Odyssey
 */
UCLASS()
class CELESTIALODYSSEY_API UCOGameEnums : public UObject
{
    GENERATED_BODY()
};

/**
 * @enum ECOGameLevel
 * @brief Defines the different levels in Celestial Odyssey
 */
UENUM(BlueprintType)
enum class ECOGameLevel : uint8
{
    None UMETA(DisplayName = "None"),
    EnchantedForestMoon UMETA(DisplayName = "Enchanted Forest Moon"),
    CrystallineCaves UMETA(DisplayName = "Crystalline Caves")
};

/**
 * @enum ECOAbilitySlot
 * @brief Defines the different slots where abilities can be mapped
 */
UENUM(BlueprintType)
enum class ECOAbilitySlot : uint8
{
    None UMETA(DisplayName = "None"),
    PrimaryAbility UMETA(DisplayName = "Primary (Q/L1)"),
    SecondaryAbility UMETA(DisplayName = "Secondary (R/R1)"),
    ComboAbility UMETA(DisplayName = "Combo (Q+R/L1+R1)"), // Gravity Shift
    DashAbility UMETA(DisplayName = "Dash"),
    GroundSlamAbility UMETA(DisplayName = "Ground Slam"),
    BasicAttack UMETA(DisplayName = "Basic Attack") // Cosmic Strike
};

/**
 * @enum EInputComboState
 * @brief Defines the different states for input combo handling
 */
UENUM(BlueprintType)
enum class EInputComboState : uint8
{
    None UMETA(DisplayName = "None"),
    Combo UMETA(DisplayName = "Combo"),
    UsingAbility UMETA(DisplayName = "Using Ability")
};

/**
 * @struct FCOLevelAbilityMapping
 * @brief Defines which abilities are available in each level
 */
USTRUCT(BlueprintType)
struct CELESTIALODYSSEY_API FCOLevelAbilityMapping
{
    GENERATED_BODY()

    /** Primary ability for this level (Q/L1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<UGameplayAbility> PrimaryAbility;

    /** Secondary ability for this level (R/R1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<UGameplayAbility> SecondaryAbility;

    /** Combo ability (Gravity Shift - shared across levels) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<UGameplayAbility> ComboAbility;

    /** Core abilities that are always available */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> CoreAbilities;
};