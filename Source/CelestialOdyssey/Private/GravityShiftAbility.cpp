#include "GravityShiftAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "COPlayerCharacter.h"
#include "GameplayEffect.h"

/** Default constructor for UGravityShiftAbility */
UGravityShiftAbility::UGravityShiftAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Set default level and gravity shift duration
    GravityShiftLevel = 1;
    GravityShiftDuration = 5.0f;

    // Set ability cooldown (5 seconds)
    CooldownEffect = nullptr; // Assign this to a valid GameplayEffect in the Blueprint or constructor.
}

/**
 * @brief Activates the gravity shift ability.
 */
void UGravityShiftAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.GravityInverted")));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        // Set gravity inversion duration based on ability level
        switch (GravityShiftLevel)
        {
        case 1:
            GravityShiftDuration = 5.0f;
            break;
        case 2:
            GravityShiftDuration = 10.0f;
            break;
        case 3:
            GravityShiftDuration = 15.0f;
            break;
        default:
            GravityShiftDuration = 5.0f;
            break;
        }

        // Store original capsule size
        DefaultCapsuleRadius = Character->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
        DefaultCapsuleHeight = Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

        // Invert gravity
        Character->GetCharacterMovement()->GravityScale = -1.0f;

        // Set an upward impulse to move the character towards the ceiling
        FVector Impulse = FVector(0.f, 0.f, 5000.f);
        Character->LaunchCharacter(Impulse, true, true);

        // Rotate character mesh to align with ceiling
        RotateCharacter(Character, true);

        // Set a timer to continuously check for ceiling collision
        Character->GetWorldTimerManager().SetTimer(
            CeilingCheckTimerHandle,
            FTimerDelegate::CreateUObject(this, &UGravityShiftAbility::CheckForCeilingContact, Character),
            0.1f, // Frequency of check (every 0.1 seconds)
            true); // Loop the timer

        // Set a timer to revert gravity after the specified duration
        FTimerHandle TimerHandle;
        Character->GetWorldTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateUObject(this, &UGravityShiftAbility::RevertGravity, Character),
            GravityShiftDuration,
            false);

        // Apply cooldown effect
        if (CooldownEffect && ASC)
        {
            FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownEffect, GetAbilityLevel());
            ASC->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
        }
    }
}

/**
 * @brief Ends the gravity shift ability, reverting gravity back to normal.
 */
void UGravityShiftAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    //ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    //RotateCharacter(Character, true);

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.GravityInverted")));
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Active")));
    }
}

/**
 * @brief Reverts the gravity back to normal.
 */
void UGravityShiftAbility::RevertGravity(ACharacter* Character)
{
    if (Character)
    {
        ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(Character);
        if (PlayerCharacter)
        {
            PlayerCharacter->SetHasReachedCeiling(false);
        }

        // Revert gravity scale to original
        Character->GetCharacterMovement()->GravityScale = 1.0f;

        // Reset the character's rotation to default
        RotateCharacter(Character, false);

        // Properly end the ability
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

/**
 * @brief Rotates the character to align with gravity shift.
 */
void UGravityShiftAbility::RotateCharacter(ACharacter* Character, bool bIsGravityInverted)
{
    if (Character)
    {
        USkeletalMeshComponent* Mesh = Character->GetMesh();
        if (Mesh)
        {
            FRotator NewRotation = Mesh->GetRelativeRotation();
            FVector NewLocation = Mesh->GetRelativeLocation();

            if (bIsGravityInverted)
            {
                // Rotate character 180 degrees to invert
                NewRotation.Roll = 180.0f;

                // Move the mesh up to align with the collision capsule (correct for pivot at the feet)
                NewLocation.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
            }
            else
            {
                // Reset character orientation
                NewRotation.Roll = 0.0f;

                // Move the mesh back to the original position
                NewLocation.Z -= Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
            }

            // Apply the new rotation and location
            Mesh->SetRelativeRotation(NewRotation);
            Mesh->SetRelativeLocation(NewLocation);
        }
    }
}

/**
 * @brief Determines if the ability can be activated.
 */
bool UGravityShiftAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.GravityShift"))))
        {
            return false;
        }

        // Check if any ability is currently casting
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Checks if the character has reached the ceiling by performing a line trace upwards.
 *
 * This function is called periodically after activating the gravity shift ability. It performs a line trace
 * to detect if there is a ceiling above the character. If the ceiling is reached, it stops further checking
 * and sets a flag indicating that the ceiling was reached.
 *
 * @param Character The character whose ceiling collision is being checked.
 */

void UGravityShiftAbility::CheckForCeilingContact(ACharacter* Character)
{
    if (Character)
    {
        ACOPlayerCharacter* PlayerCharacter = Cast<ACOPlayerCharacter>(Character);
        // Line trace to detect ceiling collision
        FVector Start = Character->GetActorLocation();
        FVector End = Start + FVector(0.f, 0.f, 500.f); // Trace 500 units upward 

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(Character); // Ignore the character itself

        // Perform the line trace
        bool bHit = Character->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

        if (PlayerCharacter && bHit && HitResult.bBlockingHit)
        {
            // We hit something above, assume it's the ceiling
            PlayerCharacter->SetHasReachedCeiling(true);

            // Stop checking since we reached the ceiling
            Character->GetWorldTimerManager().ClearTimer(CeilingCheckTimerHandle);
        }
    }
}