// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AnimInstance/HawnmunEnemyAnimInstance.h"

#include "Characters/HawnmunEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UHawnmunEnemyAnimInstance::UHawnmunEnemyAnimInstance()
{

}

void UHawnmunEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CacheOwner();
}

void UHawnmunEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CacheOwner();
	if (HawnmunEnemy == nullptr || MovementComponent == nullptr) return;

	InternalVelocity = MovementComponent->Velocity;
	InternalAcceleration = MovementComponent->GetCurrentAcceleration();
	InternalRotation = HawnmunEnemy->GetActorRotation();
}

void UHawnmunEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	UpdateMovementData();
}

void UHawnmunEnemyAnimInstance::CacheOwner()
{
	if (!HawnmunEnemy)
		HawnmunEnemy = Cast<AHawnmunEnemy>(TryGetPawnOwner());

	if (HawnmunEnemy && !MovementComponent)
		MovementComponent = HawnmunEnemy->GetCharacterMovement();
}

void UHawnmunEnemyAnimInstance::UpdateMovementData()
{
	Velocity = InternalVelocity;
	GroundSpeed = Velocity.Size2D();

	constexpr float MoveThreshold = 3.f;

	Direction = CalculateDirection(FVector(Velocity.X, Velocity.Y, 0.f), InternalRotation);
	Direction = FRotator::NormalizeAxis(Direction);
}
