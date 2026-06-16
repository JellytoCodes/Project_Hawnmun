// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AnimInstance/HawnmunPlayerAnimInstance.h"

#include "Characters/HawnmunPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHawnmunPlayerAnimInstance::UHawnmunPlayerAnimInstance()
	: OrientationAngle(0.f, 0.f, 0.f, 0.f)
	, TurnInPlaceData(0.f, 0.f, 0.f, 0.f, 0.f, 0.f)
{
}

void UHawnmunPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CacheOwner();
}

void UHawnmunPlayerAnimInstance::CacheOwner()
{
	if (!HawnmunPlayer)
		HawnmunPlayer = Cast<AHawnmunPlayer>(TryGetPawnOwner());

	if (HawnmunPlayer && !MovementComponent)
		MovementComponent = HawnmunPlayer->GetCharacterMovement();
}

void UHawnmunPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CacheOwner();
	if (!HawnmunPlayer || !MovementComponent) return;

	InternalVelocity		= MovementComponent->Velocity;
	InternalAcceleration	= MovementComponent->GetCurrentAcceleration();
	InternalRotation		= HawnmunPlayer->GetActorRotation();

	InternalCrouching		= MovementComponent->IsCrouching();
	InternalIsFalling		= MovementComponent->IsFalling();
	InternalIsSprinting		= HawnmunPlayer->GetIsSprinting();

	IsTurnCurveValue		= GetCurveValue(TEXT("IsTurn"));
	DistanceCurveValue		= GetCurveValue(TEXT("DistanceCurve"));
}

void UHawnmunPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	UpdateMovementData();
	CalcTurnInPlace();
}

void UHawnmunPlayerAnimInstance::UpdateMovementData()
{
	Velocity	= InternalVelocity;
	GroundSpeed = Velocity.Size2D();

	constexpr float MoveThreshold = 3.f;
	ShouldMove = (GroundSpeed > MoveThreshold);

	bIsFalling = InternalIsFalling;
	bIsCrouching = InternalCrouching;
	bIsSprinting = InternalIsSprinting;

	Direction = CalculateDirection(FVector(Velocity.X, Velocity.Y, 0.f), InternalRotation);
	Direction = FRotator::NormalizeAxis(Direction);

	MovementInput = EMovementInput::Forward;

	if (Direction > -45.f && Direction <= 45.f)
	{
		MovementInput = EMovementInput::Forward;
	}
	else if (Direction > 45.f && Direction <= 135.f)
	{
		MovementInput = EMovementInput::Right;
	}
	else if (Direction > 135.f || Direction <= -135.f)
	{
		MovementInput = EMovementInput::Backward;
	}
	else
	{
		MovementInput = EMovementInput::Left;
	}

	OrientationAngle.Forward	= FRotator::NormalizeAxis(Direction - 0.f);
	OrientationAngle.Right		= FRotator::NormalizeAxis(Direction - 90.f);
	OrientationAngle.Backward	= FRotator::NormalizeAxis(Direction - 180.f);
	OrientationAngle.Left		= FRotator::NormalizeAxis(Direction + 90.f);
}

void UHawnmunPlayerAnimInstance::CalcTurnInPlace()
{
	TurnInPlaceData.YawLastTick = TurnInPlaceData.Yaw;
	TurnInPlaceData.Yaw = InternalRotation.Yaw;

	TurnInPlaceData.YawChangeOver = FRotator::NormalizeAxis(TurnInPlaceData.YawLastTick - TurnInPlaceData.Yaw);

	if (ShouldMove || bIsFalling)
	{
		TurnInPlaceData.YawOffset = 0.f;
		bTurnDoOnceTriggered = false;
		return;
	}

	TurnInPlaceData.YawOffset = FRotator::NormalizeAxis(TurnInPlaceData.YawChangeOver + TurnInPlaceData.YawOffset);

	const bool bIsTurning = (IsTurnCurveValue > 0.5f);
	if (!bIsTurning)
	{
		bTurnDoOnceTriggered = false;
		return;
	}

	if (!bTurnDoOnceTriggered)
	{
		TurnInPlaceData.CurveValue = DistanceCurveValue;
		bTurnDoOnceTriggered = true;
	}

	TurnInPlaceData.LastCurveValue = TurnInPlaceData.CurveValue;
	TurnInPlaceData.CurveValue = DistanceCurveValue;

	const float CurveDelta = FMath::Abs(TurnInPlaceData.LastCurveValue - TurnInPlaceData.CurveValue);
	const float RecoverySign = (TurnInPlaceData.YawOffset >= 0.f) ? 1.f : -1.f;

	TurnInPlaceData.YawOffset -= (CurveDelta * RecoverySign);
	TurnInPlaceData.YawOffset = FRotator::NormalizeAxis(TurnInPlaceData.YawOffset);
}
