// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/HawnmunEnumTypes.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunPlayerAnimInstance.generated.h"

class AHawnmunPlayer;
class UCharacterMovementComponent;

UCLASS()
class PROJECTH_API UHawnmunPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHawnmunPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	void CacheOwner();
	void UpdateMovementData();
	void CalcTurnInPlace();

	UPROPERTY(Transient)
	TObjectPtr<AHawnmunPlayer> HawnmunPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	FVector  InternalVelocity = FVector::ZeroVector;
	FVector  InternalAcceleration = FVector::ZeroVector;
	FRotator InternalRotation = FRotator::ZeroRotator;

	bool	InternalCrouching = false;
	bool	InternalIsFalling = false;
	bool	InternalIsSprinting = false;

	float IsTurnCurveValue = 0.f;
	float DistanceCurveValue = 0.f;

	bool bTurnDoOnceTriggered = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool ShouldMove = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	EMovementInput MovementInput = EMovementInput::Forward;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FOrientationAngle OrientationAngle;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FTurnInPlaceData TurnInPlaceData;
};
