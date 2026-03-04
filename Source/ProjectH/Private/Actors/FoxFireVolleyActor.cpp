// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Actors/FoxFireVolleyActor.h"

#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Interfaces/PlayerInterface.h"

AFoxFireVolleyActor::AFoxFireVolleyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
	SetRootComponent(PatrolSpline);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(Mesh);
	CollisionSphere->SetSphereRadius(100.f);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));

}

void AFoxFireVolleyActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!PatrolSpline) return;

	CurrentDistance += MoveSpeed * DeltaSeconds;
	const float TotalLength = PatrolSpline->GetSplineLength();
	CurrentDistance = FMath::Fmod(CurrentDistance, TotalLength);

	FVector NewLocation = PatrolSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FRotator NewRotation = PatrolSpline->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	Mesh->SetWorldLocationAndRotation(NewLocation, NewRotation);
}

void AFoxFireVolleyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		
	}
}
