// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/PatrolActor.h"
#include "Characters/HawnmunEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Interfaces/PlayerInterface.h"

#pragma region Core And Initialization
APatrolActor::APatrolActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
	SetRootComponent(PatrolSpline);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(PatrolSpline);
	CollisionSphere->SetSphereRadius(55.f);
}

void APatrolActor::BeginPlay()
{
	Super::BeginPlay();
	
	PullSpawnEnemies();
}
#pragma endregion

#pragma region Patrol
void APatrolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PatrolSpline) return;

	CurrentDistance += MoveSpeed * DeltaTime;
	
	const float TotalLength = PatrolSpline->GetSplineLength();
	const float PingPongDistance = TotalLength - FMath::Abs(FMath::Fmod(CurrentDistance, TotalLength * 2.f) - TotalLength);

	const FVector SplineLocation = PatrolSpline->GetLocationAtDistanceAlongSpline(PingPongDistance, ESplineCoordinateSpace::World);
	const FRotator SplineRotation = PatrolSpline->GetRotationAtDistanceAlongSpline(PingPongDistance, ESplineCoordinateSpace::World);

	const FVector NewLocation(SplineLocation.X, SplineLocation.Y, CollisionSphere->GetComponentLocation().Z);
	
	CollisionSphere->SetWorldLocationAndRotation(NewLocation, SplineRotation);
}
#pragma endregion

#pragma region Spawner And Interaction
void APatrolActor::TriggerSpawnEnemies(AActor* InActor)
{
	if (!IsValid(InActor)) return;

	if (InActor->Implements<UPlayerInterface>())
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		for (auto& Enemy : SpawnEnemies)
		{
			Enemy->SetEnemyActive(true, InActor);
		}
		
		Destroy();
	}
}

void APatrolActor::PullSpawnEnemies()
{
if (SpawnLocations.IsEmpty()) return;
	if (SpawnEnemyClass.IsNull()) return;

	UClass* Cls = SpawnEnemyClass.LoadSynchronous();
	if (!IsValid(Cls)) return; 

	for (auto& SpawnLocation : SpawnLocations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation() + SpawnLocation);

		ESpawnActorCollisionHandlingMethod CollisionMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (AHawnmunEnemy* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AHawnmunEnemy>(Cls, SpawnTransform, nullptr, nullptr, CollisionMethod))
		{
			SpawnedEnemy->SetStartInactiveForPooling();
			SpawnedEnemy->FinishSpawning(SpawnTransform);

			SpawnedEnemy->SetEnemyActive(false);
			SpawnEnemies.Add(SpawnedEnemy);
		}
	}
}
#pragma endregion