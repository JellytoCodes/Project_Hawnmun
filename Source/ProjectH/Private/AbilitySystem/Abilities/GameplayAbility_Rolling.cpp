// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/Abilities/GameplayAbility_Rolling.h"

#include "MotionWarpingComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectH/ProjectH.h"

void UGameplayAbility_Rolling::ComputeRollDirectionAndDistance()
{
	AHawnmunPlayer* Player = GetHawnmunCharacterFromActorInfo();

	checkf(Player, TEXT("Rolling failed: AvatarActor is not AHawnmunPlayer."));
	checkf(Player->MotionWarpingComponent, TEXT("Rolling failed: MotionWarpingComponent is null on %s."), *GetNameSafe(Player));
	checkf(RollingDistance > 0.f, TEXT("Rolling failed: RollingDistance must be greater than 0. RollingDistance=%f"), RollingDistance);

	FRotator CameraRotation = Player->GetActorRotation();
	if (AController* Controller = Player->GetController())
	{
		CameraRotation = Controller->GetControlRotation();
	}

	CameraRotation.Pitch = 0.f;
	CameraRotation.Roll = 0.f;

	FVector CameraForward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

	CameraForward.Z = 0.f;
	CameraRight.Z = 0.f;

	CameraForward = CameraForward.GetSafeNormal();
	CameraRight = CameraRight.GetSafeNormal();

	FVector RollDirection = Player->GetLastMovementInputVector();
	RollDirection.Z = 0.f;

	if (RollDirection.IsNearlyZero())
	{
		RollDirection = CameraForward;
	}

	RollDirection = RollDirection.GetSafeNormal();

	const float ForwardDot = FVector::DotProduct(RollDirection, CameraForward);
	const float RightDot = FVector::DotProduct(RollDirection, CameraRight);

	if (FMath::Abs(ForwardDot) >= FMath::Abs(RightDot))
	{
		RollSection = ForwardDot >= 0.f ? ERollSection::Forward : ERollSection::Backward;
	}
	else
	{
		RollSection = RightDot >= 0.f ? ERollSection::Right : ERollSection::Left;
	}

	Player->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("RollingDirection"),
		Player->GetActorLocation(),
		UKismetMathLibrary::MakeRotFromX(RollDirection)
	);

	FVector TraceStartLocation;
	FVector TraceEndLocation;
	GetActorStartEndLocation(RollDirection, TraceStartLocation, TraceEndLocation);

	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Environment);

	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(RollGroundTrace), false, Player);

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		TraceStartLocation,
		TraceEndLocation,
		ObjectQueryParams,
		CollisionQueryParams
	);

	if (bHit && HitResult.IsValidBlockingHit())
	{
		Player->MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(
			FName("RollTargetLocation"),
			HitResult.ImpactPoint
		);
	}
	else
	{
		Player->MotionWarpingComponent->RemoveWarpTarget(FName("RollTargetLocation"));
	}
}

void UGameplayAbility_Rolling::GetActorStartEndLocation(const FVector& RollDirection, FVector& StartLocation, FVector& EndLocation)
{
	AHawnmunPlayer* Player = GetHawnmunCharacterFromActorInfo();

	checkf(Player, TEXT("GetActorStartEndLocation failed: AvatarActor is not AHawnmunPlayer."));

	StartLocation = Player->GetActorLocation() + (RollDirection * RollingDistance);
	EndLocation = StartLocation + (Player->GetActorUpVector() * -500.f);
}