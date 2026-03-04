// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoxFireVolleyActor.generated.h"

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
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> PatrolSpline;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float MoveSpeed = 300.f; // 기본값 설정 [cite: 2026-03-02]

    float CurrentDistance = 0.f;
};
