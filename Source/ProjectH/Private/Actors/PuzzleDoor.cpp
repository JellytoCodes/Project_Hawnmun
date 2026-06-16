// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/PuzzleDoor.h"
#include "Actors/InteractProp.h"
#include "Components/StaticMeshComponent.h"

#pragma region Core And Initialization
APuzzleDoor::APuzzleDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(GetRootComponent());
}

void APuzzleDoor::BeginPlay()
{
	Super::BeginPlay();

	TargetInteractCount = LinkedProps.Num();

	for (AInteractProp* Prop : LinkedProps)
	{
		if (IsValid(Prop))
		{
			Prop->OnPropInteractedDelegate.AddDynamic(this, &APuzzleDoor::OnPropInteracted);
		}
	}
}
#pragma endregion

#pragma region Puzzle Logic
void APuzzleDoor::OnPropInteracted()
{
	CurrentInteractCount++;
	if (CurrentInteractCount == TargetInteractCount)
	{
		OpenDoorEvent();
	}
}
#pragma endregion