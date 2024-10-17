#include "CrystalGrowthAbility.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

/** Default constructor for UCrystalGrowthAbility */
UCrystalGrowthAbility::UCrystalGrowthAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Set default level and crystal duration
	GrowthLevel = 1;
	CrystalDuration = 15.0f;

	StructureType = ECrystalStructureType::Bridge; // Default to bridge
}

/**
 * @brief Activates the Crystal Growth ability.
 *
 * This function allows the player to create crystal formations in the aimed direction.
 * Depending on the growth level, different types of formations can be generated.
 *
 * @param Handle Handle that identifies the ability being activated.
 * @param ActorInfo Information about the actor that is activating the ability.
 * @param ActivationInfo Information about the context of the ability activation.
 * @param TriggerEventData Event data associated with the activation trigger, if any.
 */
void UCrystalGrowthAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		//Get the direction to spawn the crystal based on the character's aiming direction
		FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 200.0f;

        if (GrowthLevel == 3)
        {
            // TODO: Replace with Gameplay Targeting and Effect to encase enemies in crystals if targeted.
            // Placeholder logic for encasing enemies.
            FHitResult HitResult;
            FVector Start = Character->GetActorLocation();
            FVector End = Start + Character->GetActorForwardVector() * 500.0f;

            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(Character);
            bool bHit = Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

            if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(FName("Enemy")))
            {
                // TODO: Replace manual encasement with a Gameplay Effect for consistency with GAS.
                AActor* HitActor = HitResult.GetActor();
                UE_LOG(LogTemp, Log, TEXT("Encasing enemy in crystal at %s"), *HitResult.Location.ToString());
                EncaseEnemy(HitActor);
            }
            else
            {
                // If no target is found or the target is not an enemy, spawn the selected structure
                SpawnCrystalFormation(SpawnLocation, GrowthLevel);
            }
        }
        else
        {
            // If GrowthLevel is 1 or 2, directly spawn the selected structure
            SpawnCrystalFormation(SpawnLocation, GrowthLevel);
        }

        // Set a timer to destroy the crystal after the duration ends
        FTimerHandle CrystalTimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            CrystalTimerHandle,
            [this, SpawnLocation]()
        {
            // TODO: Destroy the crystal formation here
            UE_LOG(LogTemp, Log, TEXT("Crystal formation at %s destroyed."), *SpawnLocation.ToString());
        },
            CrystalDuration,
            false);

        // End the ability after activation
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

/**
 * @brief Spawns the crystal formations based on the current growth level.
 *
 * This function is responsible for creating bridges, platforms, walls, or more complex structures.
 *
 * @param SpawnLocation The location where the crystal formation will spawn.
 * @param Level The level of the crystal growth, defining the type of formation.
 */
void UCrystalGrowthAbility::SpawnCrystalFormation(const FVector& SpawnLocation, int32 Level)
{
    // TODO: Replace structure spawning level logic with Gameplay Tags to manage progression.
    switch (StructureType)
    {
    case ECrystalStructureType::Bridge:
        if (Level >= 1)
        {
            UE_LOG(LogTemp, Log, TEXT("Spawning simple crystal bridge at %s"), *SpawnLocation.ToString());
            // TODO: Implement logic to spawn simple bridge/platform
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Bridge structure not available at level %d"), Level);
        }
        break;

    case ECrystalStructureType::Wall:
        if (Level >= 2)
        {
            UE_LOG(LogTemp, Log, TEXT("Spawning crystal wall at %s"), *SpawnLocation.ToString());
            // TODO: Implement logic to spawn wall
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Wall structure not available at level %d"), Level);
        }
        break;

    case ECrystalStructureType::Staircase:
        if (Level >= 2)
        {
            UE_LOG(LogTemp, Log, TEXT("Spawning spiral staircase at %s"), *SpawnLocation.ToString());
            // TODO: Implement logic to spawn staircase or arch
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Staircase structure not available at level %d"), Level);
        }
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown crystal structure type"));
        break;
    }
}

/**
 * @brief Encases an enemy in crystal.
 *
 * This function creates a crystal that encases the target enemy actor.
 *
 * @param Target The enemy actor to be encased in crystal.
 */
void UCrystalGrowthAbility::EncaseEnemy(AActor* Target)
{
    if (Target)
    {
        UE_LOG(LogTemp, Log, TEXT("Encasing enemy %s in crystal."), *Target->GetName());
        // TODO: Implement logic to encase the enemy in crystal, using a visual effect or gameplay mechanic.
    }
}
