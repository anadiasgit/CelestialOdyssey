#include "COPlayerState.h"
#include "AbilityInputEnum.h"

/**
 * @brief Constructs an instance of ACOPlayerState.
 */
ACOPlayerState::ACOPlayerState()
{
    // Initialize components
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AttributeSet = CreateDefaultSubobject<UCOPlayerAttributeSet>(TEXT("AttributeSet"));

    // Initialize states
    CurrentInputComboState = EInputComboState::None;
    CurrentLevel = ECOGameLevel::None;
}

/**
 * @brief Called when the game starts.
 */
void ACOPlayerState::BeginPlay()
{
    Super::BeginPlay();

    // Initialize player attributes from the data table
    InitializeAttributes();

    // Set up the Ability System Component with owner and avatar actor
    AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

    // Test Forest Moon abilities
    SetCurrentLevel(ECOGameLevel::EnchantedForestMoon);

    // Toggle this comment to test Crystal Cave abilities
    // SetCurrentLevel(ECOGameLevel::CrystallineCaves);

    // Update abilities based on starting level (if set)
    if (CurrentLevel != ECOGameLevel::None)
    {
        UpdateAvailableAbilities();
    }
}

/**
 * @brief Gets the Ability System Component.
 * @return Pointer to the UAbilitySystemComponent.
 */
UAbilitySystemComponent* ACOPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

/**
 * @brief Initializes the player's attributes using values from the data table.
 */
void ACOPlayerState::InitializeAttributes()
{
    if (AbilitySystemComponent && AttributeDataTable)
    {
        AbilitySystemComponent->InitStats(UCOPlayerAttributeSet::StaticClass(), AttributeDataTable);
    }
}

/**
 * @brief Sets the current input combo state for the player.
 * @param NewState The new state to set.
 */
void ACOPlayerState::SetCurrentInputComboState(EInputComboState NewState)
{
    if (CurrentInputComboState != NewState)
    {
        UE_LOG(LogTemp, Log, TEXT("Changing input combo state from %d to %d"),
            (uint8)CurrentInputComboState, (uint8)NewState);
    }
    CurrentInputComboState = NewState;
}

/**
 * @brief Changes the current level and updates available abilities.
 * @param NewLevel The level to switch to.
 */
void ACOPlayerState::SetCurrentLevel(ECOGameLevel NewLevel)
{
    if (CurrentLevel != NewLevel)
    {
        CurrentLevel = NewLevel;
        UpdateAvailableAbilities();
    }
}

/**
 * @brief Gets the ability assigned to a specific slot for the current level.
 * @param Slot The ability slot to query.
 * @return The gameplay ability class assigned to that slot, or nullptr if none.
 */
TSubclassOf<UGameplayAbility> ACOPlayerState::GetAbilityForSlot(ECOAbilitySlot Slot) const
{
    if (const FCOLevelAbilityMapping* Mapping = LevelAbilityMappings.Find(CurrentLevel))
    {
        switch (Slot)
        {
        case ECOAbilitySlot::PrimaryAbility:
            return Mapping->PrimaryAbility;
        case ECOAbilitySlot::SecondaryAbility:
            return Mapping->SecondaryAbility;
        case ECOAbilitySlot::ComboAbility:
            return Mapping->ComboAbility;
        case ECOAbilitySlot::DashAbility:
            return Mapping->CoreAbilities.Contains(CelestialDashAbilityClass) ?
                CelestialDashAbilityClass : nullptr;
        case ECOAbilitySlot::GroundSlamAbility:
            return Mapping->CoreAbilities.Contains(GroundSlamAbilityClass) ?
                GroundSlamAbilityClass : nullptr;
        case ECOAbilitySlot::BasicAttack:
            return Mapping->CoreAbilities.Contains(CosmicStrikeAbilityClass) ?
                CosmicStrikeAbilityClass : nullptr;
        default:
            return nullptr;
        }
    }
    return nullptr;
}

/**
 * @brief Updates available abilities based on current level.
 */
void ACOPlayerState::UpdateAvailableAbilities()
{
    if (!AbilitySystemComponent || CurrentLevel == ECOGameLevel::None)
    {
        return;
    }

    // Remove existing abilities
    AbilitySystemComponent->ClearAllAbilities();

    // Get ability mapping for current level
    if (const FCOLevelAbilityMapping* Mapping = LevelAbilityMappings.Find(CurrentLevel))
    {
        // Grant primary ability
        if (Mapping->PrimaryAbility)
        {
            AbilitySystemComponent->GiveAbility(
                FGameplayAbilitySpec(Mapping->PrimaryAbility, 1,
                    static_cast<int32>(EAbilityInput::VineWhip)));  // Adjust input binding as needed
        }

        // Grant secondary ability
        if (Mapping->SecondaryAbility)
        {
            AbilitySystemComponent->GiveAbility(
                FGameplayAbilitySpec(Mapping->SecondaryAbility, 1,
                    static_cast<int32>(EAbilityInput::LunarForestFury)));  // Adjust input binding as needed
        }

        // Grant combo ability
        if (Mapping->ComboAbility)
        {
            AbilitySystemComponent->GiveAbility(
                FGameplayAbilitySpec(Mapping->ComboAbility, 1,
                    static_cast<int32>(EAbilityInput::GravityShift)));
        }

        // Grant core abilities
        for (const auto& CoreAbility : Mapping->CoreAbilities)
        {
            if (CoreAbility)
            {
                int32 InputID = 0;
                // Determine proper input binding based on ability type
                if (CoreAbility == CelestialDashAbilityClass)
                    InputID = static_cast<int32>(EAbilityInput::CelestialDash);
                else if (CoreAbility == GroundSlamAbilityClass)
                    InputID = static_cast<int32>(EAbilityInput::GroundSlam);
                else if (CoreAbility == CosmicStrikeAbilityClass)
                    InputID = static_cast<int32>(EAbilityInput::CosmicStrike);

                AbilitySystemComponent->GiveAbility(
                    FGameplayAbilitySpec(CoreAbility, 1, InputID));
            }
        }
    }


    if (const FCOLevelAbilityMapping* Mapping = LevelAbilityMappings.Find(CurrentLevel))
    {
        // When granting abilities, add debug output
        if (Mapping->PrimaryAbility)
        {
            UE_LOG(LogTemp, Log, TEXT("Granting Primary Ability: %s"),
                *Mapping->PrimaryAbility->GetName());
        }

        if (Mapping->SecondaryAbility)
        {
            UE_LOG(LogTemp, Log, TEXT("Granting Secondary Ability: %s"),
                *Mapping->SecondaryAbility->GetName());
        }
    }
}