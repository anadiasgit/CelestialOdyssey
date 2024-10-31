#include "GravityShiftAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
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

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.GravityInverted")));
    }
}

/**
 * @brief Reverts the gravity back to normal.
 */
void UGravityShiftAbility::RevertGravity(ACharacter* Character)
{
    if (Character)
    {
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
            if (bIsGravityInverted)
            {
                NewRotation.Roll = 180.0f;
            }
            else
            {
                NewRotation.Roll = 0.0f;
            }
            Mesh->SetRelativeRotation(NewRotation);
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
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.OnCooldown"))) ||
            ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
        {
            return false;
        }
    }

    return true;
}