// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailed.h"

#include "Actors/FoxFireDropActor.h"
#include "Actors/PortalBreathActor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectH/ProjectH.h"

AHawnmunNineTailed::AHawnmunNineTailed()
{

}

int32 AHawnmunNineTailed::GetCharacterLevel_Implementation()
{
	return 1;
}

void AHawnmunNineTailed::Die()
{
	//
}

APortalBreathActor* AHawnmunNineTailed::GetRandomPortalBreath()
{
	int32 RendIdx = FMath::RandRange(0, 3);
	return FieldPortalBreath[RendIdx];
}

void AHawnmunNineTailed::ActiveFoxFireCardinalDrop(const FDamageEffectParams& DamageEffectParams)
{
	for (auto& DropActor : FoxFireCardinalDrop)
		DropActor->ActiveEffect(DamageEffectParams);
}

void AHawnmunNineTailed::ActiveFoxFireDiagonalDrop(const FDamageEffectParams& DamageEffectParams)
{
	for (auto& DropActor : FoxFireDiagonalDrop)
		DropActor->ActiveEffect(DamageEffectParams);
}

void AHawnmunNineTailed::BeginPlay()
{
	Super::BeginPlay();

	CreatePortalBreath();
	CreateFoxFireDrops();
}

void AHawnmunNineTailed::CreatePortalBreath()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortalBreathActor::StaticClass(), FoundActors);

	for (auto* Actor : FoundActors)
	{
		if (APortalBreathActor* PortalBreath = Cast<APortalBreathActor>(Actor))
		{
			PortalBreath->AddIgnoreActor(this);
			FieldPortalBreath.Add(PortalBreath);
		}
	}
}

void AHawnmunNineTailed::CreateFoxFireDrops()
{
	// + 패턴
	for (int i = 0 ; i < CardinalDrop.Num(); i++)
	{
		if (UClass* Cls = SpawnClass.LoadSynchronous())
		{
			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.SetLocation(GetActorLocation() + CardinalDrop[i]);

			if (AFoxFireDropActor* SpawnedFoxFireDrop = GetWorld()->SpawnActorDeferred<AFoxFireDropActor>(Cls, SpawnTransform))
			{
				SpawnedFoxFireDrop->AddIgnoreActor(this);
				SpawnedFoxFireDrop->FinishSpawning(SpawnTransform);
				FoxFireCardinalDrop.Add(SpawnedFoxFireDrop);
			}
		}
	}

	// x 패턴
	for (int i = 0; i < DiagonalDrop.Num(); i++)
	{
		if (UClass* Cls = SpawnClass.LoadSynchronous())
		{
			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.SetLocation(GetActorLocation() + DiagonalDrop[i]);

			if (AFoxFireDropActor* SpawnedFoxFireDrop = GetWorld()->SpawnActorDeferred<AFoxFireDropActor>(Cls, SpawnTransform))
			{
				SpawnedFoxFireDrop->AddIgnoreActor(this);
				SpawnedFoxFireDrop->FinishSpawning(SpawnTransform);
				FoxFireDiagonalDrop.Add(SpawnedFoxFireDrop);
			}
		}
	}
}
