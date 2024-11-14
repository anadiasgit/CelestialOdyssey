#include "CrystalShatterAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCrystalShatterAbility::UCrystalShatterAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Initialize default values
    ShatterLevel = 1;
    SlowFieldDuration = 5.0f;
    ShatterRadius = 500.0f;
    BaseDamage = 50.0f;
}

// Continuing CrystalShatterAbility.cpp

void UCrystalShatterAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!ActorInfo)
        return;

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
        return;

    // Add casting tag
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));

        // Apply cooldown
        if (CooldownEffectClass)
        {
            FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownEffectClass, GetAbilityLevel());
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }

    // Perform the shatter effect at the character's location
    PerformShatter(Character->GetActorLocation());

    // End the ability after the shatter effect
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCrystalShatterAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting")));
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UCrystalShatterAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
        return false;

    if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.CrystalShatter"))))
            return false;

        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Casting"))))
            return false;
    }

    return true;
}

void UCrystalShatterAbility::PerformShatter(const FVector& Location)
{
    if (!GetWorld())
        return;

    // Adjust effects based on ability level
    float CurrentRadius = ShatterRadius;
    float CurrentDamage = BaseDamage;

    switch (ShatterLevel)
    {
    case 2:
        CurrentRadius *= 1.5f;
        CurrentDamage *= 1.25f;
        break;
    case 3:
        CurrentRadius *= 2.0f;
        CurrentDamage *= 1.5f;
        break;
    }

    // Detect affected actors
    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(CurrentRadius);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwningActorFromActorInfo());

    if (GetWorld()->SweepMultiByChannel(HitResults, Location, Location, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams))
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor || HitActor == GetOwningActorFromActorInfo())
                continue;

            if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
            {
                // Apply initial damage
                if (DamageEffectClass)
                {
                    FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
                    DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), CurrentDamage);
                    TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
                }

                // Apply DoT for level 2+
                if (ShatterLevel >= 2 && DoTEffectClass)
                {
                    FGameplayEffectSpecHandle DoTSpec = MakeOutgoingGameplayEffectSpec(DoTEffectClass, GetAbilityLevel());
                    TargetASC->ApplyGameplayEffectSpecToSelf(*DoTSpec.Data.Get());
                }
            }
        }
    }

    // Create slow field
    CreateSlowField(Location);

    // Launch fragments for level 3
    if (ShatterLevel >= 3)
    {
        LaunchFragments(Location);
    }
}

void UCrystalShatterAbility::CreateSlowField(const FVector& Location)
{
    if (!GetWorld())
        return;

    FTimerDelegate PeriodicCheckDelegate;
    PeriodicCheckDelegate.BindLambda([this, Location]()
    {
        if (!GetWorld())
            return;

        TArray<FHitResult> HitResults;
        FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ShatterRadius);
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetOwningActorFromActorInfo());

        if (GetWorld()->SweepMultiByChannel(HitResults, Location, Location, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams))
        {
            for (const FHitResult& Hit : HitResults)
            {
                if (UAbilitySystemComponent* TargetASC = Hit.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
                {
                    if (SlowEffectClass)
                    {
                        FGameplayEffectSpecHandle SlowSpec = MakeOutgoingGameplayEffectSpec(SlowEffectClass, GetAbilityLevel());
                        TargetASC->ApplyGameplayEffectSpecToSelf(*SlowSpec.Data.Get());
                    }
                }
            }
        }
    });

    // Start the periodic check
    GetWorld()->GetTimerManager().SetTimer(
        SlowFieldTimerHandle,
        PeriodicCheckDelegate,
        0.5f,  // Check every half second
        true   // Looping
    );

    // Create timer to clear the slow field
    FTimerHandle CleanupTimerHandle;
    FTimerDelegate CleanupDelegate;
    CleanupDelegate.BindLambda([this]()
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(SlowFieldTimerHandle);
        }
    });

    GetWorld()->GetTimerManager().SetTimer(
        CleanupTimerHandle,
        CleanupDelegate,
        SlowFieldDuration,
        false
    );
}

void UCrystalShatterAbility::LaunchFragments(const FVector& Origin)
{
    const int32 NumFragments = 8; // Number of fragments to launch
    const float FragmentSpeed = 500.0f;

    for (int32 i = 0; i < NumFragments; ++i)
    {
        // Calculate fragment direction (evenly spaced around a circle)
        float Angle = (2.0f * PI * i) / NumFragments;
        FVector Direction(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);

        // Spawn fragment actor/projectile (implement in Blueprint)
        // The fragment should have its own collision detection for ricochets
    }
}