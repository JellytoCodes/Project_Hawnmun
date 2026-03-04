// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoxFireVolleyActor.generated.h"

class AHawnmunEnemy;
class USphereComponent;
class USplineComponent;

UCLASS()
class PROJECTH_API AFoxFireVolleyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoxFireVolleyActor();

	virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;
	
	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    void PullSpawnEnemies();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> PatrolSpline;

    UPROPERTY(EditDefaultsOnly, Category = "Patrol")
    TSoftClassPtr<AHawnmunEnemy> SpawnEnemyClass;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float MoveSpeed = 100.f;

    float CurrentDistance = 0.f;

    UPROPERTY(EditAnywhere, Category = "Patrol", meta = (MakeEditWidget = true))
    TArray<FVector> SpawnLocations;

    UPROPERTY()
    TArray<TObjectPtr<AHawnmunEnemy>> SpawnEnemies;
};
