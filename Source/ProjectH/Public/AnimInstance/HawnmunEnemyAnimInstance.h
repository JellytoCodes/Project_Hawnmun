// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HawnmunEnemyAnimInstance.generated.h"

class AHawnmunEnemy;
class UCharacterMovementComponent;

UCLASS()
class PROJECTH_API UHawnmunEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public :
	UHawnmunEnemyAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private :
	void CacheOwner();
	void UpdateMovementData();

	UPROPERTY(Transient)
	TObjectPtr<AHawnmunEnemy> HawnmunEnemy;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	FVector  InternalVelocity = FVector::ZeroVector;
	FVector  InternalAcceleration = FVector::ZeroVector;
	FRotator InternalRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction = 0.f;
};
