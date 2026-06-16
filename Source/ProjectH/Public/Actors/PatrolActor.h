// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolActor.generated.h"

class AHawnmunEnemy;
class USphereComponent;
class USplineComponent;

UCLASS()
class PROJECTH_API APatrolActor : public AActor
{
	GENERATED_BODY()
	
#pragma region Core And Initialization
public:	
	APatrolActor();

protected:
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Patrol
public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Patrol")
	TObjectPtr<USplineComponent> PatrolSpline;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MoveSpeed = 100.f;

	float CurrentDistance = 0.f;
#pragma endregion

#pragma region Spawner And Interaction
public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void TriggerSpawnEnemies(AActor* InActor);

private:
	void PullSpawnEnemies();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TSoftClassPtr<AHawnmunEnemy> SpawnEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnLocations;

	UPROPERTY()
	TArray<TObjectPtr<AHawnmunEnemy>> SpawnEnemies;
#pragma endregion
};