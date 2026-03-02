// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailedIllusion.h"

#include "HawnmunFunctionLibrary.h"
#include "NiagaraComponent.h"

AHawnmunNineTailedIllusion::AHawnmunNineTailedIllusion()
{
	PrimaryActorTick.bCanEverTick = false;

	CharmIllusionEffect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	CharmIllusionEffect->SetAutoActivate(false);
	CharmIllusionEffect->SetupAttachment(GetMesh());
}

void AHawnmunNineTailedIllusion::CauseDamage()
{
	TArray<AActor*> Dummy;

	UHawnmunFunctionLibrary::GetLivePlayersWithinRadius(
		this,
		Dummy,
		ActorsToIgnore,
		300.f,
		GetActorLocation());
}

void AHawnmunNineTailedIllusion::AddIgnoreActor(AActor* InActor)
{
	ActorsToIgnore.Add(InActor);
}
