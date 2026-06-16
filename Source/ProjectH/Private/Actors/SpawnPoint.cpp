// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/SpawnPoint.h"

#include "Characters/HawnmunEnemy.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (bUsePersistence && bReached) return;

	if (UClass* Cls = SpawnClass.LoadSynchronous())
	{
		FActorSpawnParameters SpawnParam;

		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Spawned = GetWorld()->SpawnActorDeferred<AActor>(Cls, GetActorTransform());
		if (Spawned.IsValid())
		{
			Spawned->FinishSpawning(GetActorTransform());
			Spawned->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnSpawnedDestroyed);

			if (AHawnmunEnemy* Enemy = Cast<AHawnmunEnemy>(Spawned))
			{
				Enemy->SetSpawnedActorByBlackBoardKey(this);
			}
		}
	}
}

void ASpawnPoint::OnSpawnedDestroyed(AActor* DestroyedActor)
{
	if (bUsePersistence) bReached = true;
}
