#include "LunarForestFuryAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ULunarForestFuryAbility::ULunarForestFuryAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    FuryLevel = 1;
    RootDuration = 8.0f;
}

void ULunarForestFuryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Log, TEXT("LunarForestFury: Starting Activation"));

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        UE_LOG(LogTemp, Log, TEXT("LunarForestFury: Found ASC"));

        // Apply cooldown effect
        if (CooldownEffect)
        {
            UE_LOG(LogTemp, Log, TEXT("LunarForestFury: Applying Cooldown"));
            FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownEffect, 1.0f);
            ASC->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("LunarForestFury: No Cooldown Effect Set"));
        }

        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        UE_LOG(LogTemp, Log, TEXT("LunarForestFury: Added Casting Tag"));
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        FVector EruptionLocation = Character->GetActorLocation();
        float BaseRadius = 300.0f;
        float Radius = FuryLevel >= 2 ? BaseRadius * 1.5f : BaseRadius;

        TArray<FHitResult> HitResults;
        FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

        if (GetWorld()->SweepMultiByChannel(HitResults, EruptionLocation, EruptionLocation,
            FQuat::Identity, ECC_Pawn, CollisionShape))
        {
            for (const FHitResult& Hit : HitResults)
            {
                AActor* HitActor = Hit.GetActor();
                if (!HitActor || HitActor == Character) continue;

                if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
                {
                    // Base damage effect
                    if (DamageGameplayEffectClass)
                    {
                        FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffectClass, GetAbilityLevel());
                        TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
                    }

                    // Level-specific effects
                    if (FuryLevel >= 2 && RootGameplayEffectClass)
                    {
                        // Root effect at level 2+
                        FGameplayEffectSpecHandle RootSpecHandle = MakeOutgoingGameplayEffectSpec(RootGameplayEffectClass, GetAbilityLevel());
                        TargetASC->ApplyGameplayEffectSpecToSelf(*RootSpecHandle.Data.Get());
                    }

                    if (FuryLevel >= 3 && DoTGameplayEffectClass)
                    {
                        // DoT effect at level 3
                        FGameplayEffectSpecHandle DoTSpecHandle = MakeOutgoingGameplayEffectSpec(DoTGameplayEffectClass, GetAbilityLevel());
                        TargetASC->ApplyGameplayEffectSpecToSelf(*DoTSpecHandle.Data.Get());
                    }

                    // Apply knockback
                    FVector KnockbackDirection = (HitActor->GetActorLocation() - EruptionLocation).GetSafeNormal();
                    KnockbackDirection.Z = 0.5f; // Add some upward force
                    float KnockbackStrength = 1000.0f;

                    if (ACharacter* HitCharacter = Cast<ACharacter>(HitActor))
                    {
                        HitCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
                    }
                }
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("LunarForestFury: Calling EndAbility"));
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void ULunarForestFuryAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Active")));
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool ULunarForestFuryAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (ASC)
    {
        // Only check for this ability's specific cooldown tag
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.LunarForestFury"))))
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