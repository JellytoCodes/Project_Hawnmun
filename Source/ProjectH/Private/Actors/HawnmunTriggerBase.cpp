// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Actors/HawnmunTriggerBase.h"
#include "Components/BoxComponent.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "HUD/HawnmunHUD.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WidgetController/CinematicWidgetController.h"
#include "ProjectH/ProjectH.h"

AHawnmunTriggerBase::AHawnmunTriggerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

void AHawnmunTriggerBase::ExecuteCinematicRequest(FGameplayTag CinematicTag, AActor* InActor)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	if (AHawnmunHUD* HawnmunHUD = Cast<AHawnmunHUD>(PC->GetHUD()))
	{
		HawnmunHUD->ShowCinematicWidget();
		HawnmunHUD->GetCinematicSlotWidgetController(FWidgetControllerParams())->RequestPlayCinematic(CinematicTag, InActor);
	}
}

void AHawnmunTriggerBase::SaveCheckpointFromActor(AActor* InActor) const
{
	if (!InActor) return;

	if (UHawnmunDungeonSubsystem* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		DungeonSubsystem->SaveCheckpoint(InActor->GetActorTransform());
	}
}
