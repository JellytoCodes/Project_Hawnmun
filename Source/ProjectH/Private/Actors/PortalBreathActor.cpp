// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/PortalBreathActor.h"
#include "HawnmunFunctionLibrary.h"

// Sets default values
APortalBreathActor::APortalBreathActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APortalBreathActor::ActiveBreath(TArray<AActor*>& OutOverlappingActors)
{
	const FVector StartLocation = GetActorLocation();
	const FRotator AttackRotation = GetActorRotation();
	
	const FVector ForwardVector = AttackRotation.Vector();
	const FVector AdjOrigin = StartLocation + (ForwardVector * BoxExtent.X);

	UHawnmunFunctionLibrary::GetLivePlayersWithinBox(
		this,
		OutOverlappingActors,
		ActorsToIgnore,
		BoxExtent,
		AttackRotation,
		AdjOrigin
	);
}

void APortalBreathActor::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}

