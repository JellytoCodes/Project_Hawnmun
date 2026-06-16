// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "Actors/DungeonSelector.h"

#include "Components/BoxComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "ProjectH/ProjectH.h"

ADungeonSelector::ADungeonSelector()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(CollisionBox);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void ADungeonSelector::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADungeonSelector::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_ShowDungeonMap(OtherActor);
	}
}

void ADungeonSelector::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{

	}
}
