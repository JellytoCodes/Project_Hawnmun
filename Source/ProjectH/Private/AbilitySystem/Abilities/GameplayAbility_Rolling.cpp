// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/Abilities/GameplayAbility_Rolling.h"

#include "MotionWarpingComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectH/ProjectH.h"

void UGameplayAbility_Rolling::ComputeRollDirectionAndDistance()
{
	AHawnmunPlayer* Char = GetHawnmunCharacterFromActorInfo();
	if (!Char || !Char->MotionWarpingComponent) return;

	FRotator CamRot = Char->GetActorRotation();
	if (AController* C = Char->GetController())
	{
		CamRot = C->GetControlRotation();
	}
	CamRot.Pitch = 0.f;
	CamRot.Roll  = 0.f;

	FVector CamForward = FRotationMatrix(CamRot).GetUnitAxis(EAxis::X);
	FVector CamRight   = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);
	CamForward.Z = 0.f;
	CamRight.Z   = 0.f;
	CamForward = CamForward.GetSafeNormal();
	CamRight   = CamRight.GetSafeNormal();

	FVector RollingDir = Char->GetLastMovementInputVector();
	RollingDir.Z = 0.f;

	if (RollingDir.IsNearlyZero())
		RollingDir = CamForward;

	RollingDir = RollingDir.GetSafeNormal();

	const float FwdDot   = FVector::DotProduct(RollingDir, CamForward); // +면 화면 전방
	const float RightDot = FVector::DotProduct(RollingDir, CamRight);   // +면 화면 우측

	if (FMath::Abs(FwdDot) >= FMath::Abs(RightDot))
		RollSection = (FwdDot >= 0.f) ? ERollSection::Forward : ERollSection::Backward;
	else
		RollSection = (RightDot >= 0.f) ? ERollSection::Right : ERollSection::Left;

	Char->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("RollingDirection"),
		FVector::ZeroVector,
		UKismetMathLibrary::MakeRotFromX(RollingDir)
	);

	FVector StartLoc = FVector::ZeroVector;
	FVector EndLoc   = FVector::ZeroVector;
	GetActorStartEndLocation(RollingDir, StartLoc, EndLoc);

	FHitResult HitResult;

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_Environment);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(RollGroundTrace), false, Char);

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLoc,
		EndLoc,
		ObjParams,
		QueryParams
	);

	if (bHit && HitResult.IsValidBlockingHit())
	{
		Char->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(
			FName("RollTargetLocation"),
			HitResult.ImpactPoint
		);
	}
	else
	{
		Char->MotionWarpingComponent->RemoveWarpTarget(FName("RollTargetLocation"));
	}
}

void UGameplayAbility_Rolling::GetActorStartEndLocation(const FVector& CachedRollingDirection, FVector& StartLoc, FVector& EndLoc)
{
	StartLoc =
		GetHawnmunCharacterFromActorInfo()->GetActorLocation() + (CachedRollingDirection * RollingDistance);

	EndLoc =
		StartLoc + ((GetHawnmunCharacterFromActorInfo()->GetActorUpVector() * -1.f) * 500.f);
}
