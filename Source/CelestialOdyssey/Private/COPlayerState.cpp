#include "COPlayerState.h"
#include "AbilityInputEnum.h"

/**
 * @brief Constructs an instance of ACOPlayerState.
 *
 * Initializes the Ability System Component for this player. This component will be responsible
 * for managing abilities and interacting with Gameplay Effects.
 */
ACOPlayerState::ACOPlayerState()
{
	//Initialize the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * Initializes the Ability System Component and assigns abilities to the player.
 */
void ACOPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		if (CelestialDashAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CelestialDashAbilityClass, 1, static_cast<int32>(EAbilityInput::CelestialDash)));
		}
		
		if (GroundSlamAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GroundSlamAbilityClass, 1, static_cast<int32>(EAbilityInput::GroundSlam)));
		}

		if (GravityShiftAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GravityShiftAbilityClass, 1, static_cast<int32>(EAbilityInput::GravityShift)));
		}

		if (CosmicStrikeAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CosmicStrikeAbilityClass, 1, static_cast<int32>(EAbilityInput::CosmicStrike)));
		}

		if (CrystalGrowthAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CrystalGrowthAbilityClass, 1, static_cast<int32>(EAbilityInput::CrystalGrowth)));
		}

		if (CrystalShatterAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CrystalShatterAbilityClass, 1, static_cast<int32>(EAbilityInput::CrystalShatter)));
		}

		if (VineWhipAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(VineWhipAbilityClass, 1, static_cast<int32>(EAbilityInput::VineWhip)));
		}

		if (LunarForestFuryAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(LunarForestFuryAbilityClass, 1, static_cast<int32>(EAbilityInput::LunarForestFury)));
		}
	}
}

/**
 * @brief Gets the Ability System Component associated with this player.
 *
 * This method is used by the Gameplay Ability System to interact with the player's abilities.
 *
 * @return Pointer to the UAbilitySystemComponent.
 */
UAbilitySystemComponent* ACOPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}