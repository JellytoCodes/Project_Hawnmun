// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/TransitionInteractActor.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectH/ProjectH.h"

ATransitionInteractActor::ATransitionInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PlatformCollision = CreateDefaultSubobject<UBoxComponent>("PlatformCollision");
	PlatformCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlatformCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(PlatformCollision);

	DestinationPoint = CreateDefaultSubobject<USceneComponent>("DestinationPoint");
	DestinationPoint->SetupAttachment(PlatformCollision);

	ViewDestinationPoint = CreateDefaultSubobject<UBillboardComponent>("ViewDestinationPoint");
	ViewDestinationPoint->SetupAttachment(DestinationPoint);
}