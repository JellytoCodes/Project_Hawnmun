// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailed.h"

#include "Actors/FoxFireDropActor.h"
#include "Actors/PortalBreathActor.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Characters/HawnmunNineTailedIllusion.h"
#include "ProjectH/ProjectH.h"
#include "Algo/RandomShuffle.h"

AHawnmunNineTailed::AHawnmunNineTailed()
{
	PrimaryActorTick.bCanEverTick = false;

	CharmIllusionEffect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	CharmIllusionEffect->SetAutoActivate(false);
	CharmIllusionEffect->SetupAttachment(GetMesh());

	CachedCenterLocation = FVector::ZeroVector;

	CardinalDropLocation.Add(FVector(1000.f, 0.f, 0.f));
	CardinalDropLocation.Add(FVector(-1000.f, 0.f, 0.f));
	CardinalDropLocation.Add(FVector(0.f, 1000.f, 0.f));
	CardinalDropLocation.Add(FVector(0.f, -1000.f, 0.f));

	DiagonalDropLocation.Add(FVector(1000.f, -1000.f, 0.f));
	DiagonalDropLocation.Add(FVector(-1000.f, -1000.f, 0.f));
	DiagonalDropLocation.Add(FVector(-1000.f, 1000.f, 0.f));
	DiagonalDropLocation.Add(FVector(1000.f, 1000.f, 0.f));
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

void AHawnmunNineTailed::ActivateIllusionsEffect()
{
	for (auto& Illusion : NineTailedIllusion)
		Illusion->ActivateCharmIllusionEffect();
}

void AHawnmunNineTailed::DeactivateIllusionsEffect()
{
	for (auto& Illusion : NineTailedIllusion)
		Illusion->DeactivateCharmIllusionEffect();
}

void AHawnmunNineTailed::ShuffleNineTailedCharmIllusion()
{
	if (NineTailedIllusionLocation.IsEmpty()) return;

	Algo::RandomShuffle(NineTailedIllusionLocation);

	int idx = 0;

	FVector RealLoc = NineTailedIllusionLocation[idx++];
	FRotator RealRot = (CachedCenterLocation - RealLoc).Rotation();
	SetActorLocationAndRotation(RealLoc, RealRot);

	for (auto& Illusion : NineTailedIllusion)
	{
		FVector IllusionLoc = NineTailedIllusionLocation[idx++];
		FRotator IllusionRot = (CachedCenterLocation - IllusionLoc).Rotation();
		IllusionRot.Pitch = 0.f;
		IllusionRot.Roll = 0.f;
		Illusion->SetActorLocationAndRotation(IllusionLoc, IllusionRot);
	}
		
}

void AHawnmunNineTailed::BackCenterLocation(const FDamageEffectParams& DamageEffectParams)
{
	SetActorLocation(CachedCenterLocation);

	for (auto& Illusion : NineTailedIllusion)
	{
		Illusion->CombatDamageEffectParams = DamageEffectParams;
		Illusion->CauseDamage();
	}
}

void AHawnmunNineTailed::BeginPlay()
{
	Super::BeginPlay();

	CreatePortalBreath();
	CreateFoxFireDrops();

	CreateNineTailedIllusion();
	GenerateCirclePoints(1500.f);

	CachedCenterLocation = GetActorLocation();
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
	for (int i = 0 ; i < CardinalDropLocation.Num(); i++)
	{
		if (UClass* Cls = SpawnFoxFireDropClass.LoadSynchronous())
		{
			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.SetLocation(GetActorLocation() + CardinalDropLocation[i]);

			if (AFoxFireDropActor* SpawnedFoxFireDrop = GetWorld()->SpawnActorDeferred<AFoxFireDropActor>(Cls, SpawnTransform))
			{
				SpawnedFoxFireDrop->AddIgnoreActor(this);
				SpawnedFoxFireDrop->FinishSpawning(SpawnTransform);
				FoxFireCardinalDrop.Add(SpawnedFoxFireDrop);
			}
		}
	}

	// x 패턴
	for (int i = 0; i < DiagonalDropLocation.Num(); i++)
	{
		if (UClass* Cls = SpawnFoxFireDropClass.LoadSynchronous())
		{
			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.SetLocation(GetActorLocation() + DiagonalDropLocation[i]);

			if (AFoxFireDropActor* SpawnedFoxFireDrop = GetWorld()->SpawnActorDeferred<AFoxFireDropActor>(Cls, SpawnTransform))
			{
				SpawnedFoxFireDrop->AddIgnoreActor(this);
				SpawnedFoxFireDrop->FinishSpawning(SpawnTransform);
				FoxFireDiagonalDrop.Add(SpawnedFoxFireDrop);
			}
		}
	}
}

void AHawnmunNineTailed::CreateNineTailedIllusion()
{
	for (int i = 0 ; i < 4 ; i++)
	{
		if (UClass* Cls = SpawnNineTailedIllusionClass.LoadSynchronous())
		{
			FTransform SpawnTransform = GetActorTransform();
			if (AHawnmunNineTailedIllusion* SpawnedNineTailedIllusion = GetWorld()->SpawnActorDeferred<AHawnmunNineTailedIllusion>(Cls, SpawnTransform))
			{
				SpawnedNineTailedIllusion->FinishSpawning(SpawnTransform);
				NineTailedIllusion.Add(SpawnedNineTailedIllusion);
			}
		}
	}

	if (NineTailedIllusion.IsEmpty() == false)
		ActivateIllusionsEffect();
}

void AHawnmunNineTailed::GenerateCirclePoints(float Radius)
{
	NineTailedIllusionLocation.Empty();

	const float AngleStep = 360.0f / 5.0f; // 72도

    for (int32 i = 0; i < 5; ++i)
    {
        float CurrentAngle = i * AngleStep;
        float Rad = FMath::DegreesToRadians(CurrentAngle);

        // X = R * cos, Y = R * sin
        FVector Offset(FMath::Cos(Rad) * Radius, FMath::Sin(Rad) * Radius, 0.f);
        NineTailedIllusionLocation.Add(GetActorLocation() + Offset);
    }
}
