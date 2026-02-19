// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/Abilities/GameplayAbility_Rolling.h"

#include "MotionWarpingComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UGameplayAbility_Rolling::ComputeRollDirectionAndDistance()
{
	FVector CachedRollingDirection = GetHawnmunCharacterFromActorInfo()->GetLastMovementInputVector();
	CachedRollingDirection.Normalize(0.0001);

	

	GetHawnmunCharacterFromActorInfo()->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("RollingDirection"), 
		FVector::ZeroVector, 
		UKismetMathLibrary::MakeRotFromX(CachedRollingDirection));

	FVector StartLoc = FVector::ZeroVector;
	FVector EndLoc = FVector::ZeroVector;
	GetActorStartEndLocation(CachedRollingDirection, StartLoc, EndLoc);

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLoc,
		EndLoc,
		ECC_WorldDynamic);

	if (HitResult.IsValidBlockingHit())
	{
		GetHawnmunCharacterFromActorInfo()->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(
			FName("RollTargetLocation"),
			HitResult.ImpactPoint);
	}
	else
	{
		GetHawnmunCharacterFromActorInfo()->MotionWarpingComponent->RemoveWarpTarget(FName("RollTargetLocation"));
	}
}

void UGameplayAbility_Rolling::GetActorStartEndLocation(const FVector& CachedRollingDirection, FVector& StartLoc, FVector& EndLoc)
{
	StartLoc =
		GetHawnmunCharacterFromActorInfo()->GetActorLocation() + (CachedRollingDirection * RollingDistance);

	EndLoc =
		StartLoc + ((GetHawnmunCharacterFromActorInfo()->GetActorUpVector() * -1.f) * 500.f);
}
