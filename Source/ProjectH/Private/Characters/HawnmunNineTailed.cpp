// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Characters/HawnmunNineTailed.h"

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

void AHawnmunNineTailed::BeginPlay()
{
	Super::BeginPlay();

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
