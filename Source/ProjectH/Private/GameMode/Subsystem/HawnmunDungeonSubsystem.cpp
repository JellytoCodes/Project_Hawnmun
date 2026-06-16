// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"

#include "ProjectH/ProjectH.h"

void UHawnmunDungeonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UHawnmunDungeonSubsystem::Deinitialize()
{
	ClearSessionData();
	Super::Deinitialize();
}

void UHawnmunDungeonSubsystem::StartDungeonSession(FGameplayTag InDungeonTag)
{
	bIsDungeonSession = true;
	ClearSessionData();

	CurrentDungeonTag = InDungeonTag;
	DungeonStartTimeSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	ClearTimeSeconds = 0.f;
	bDungeonCleared = false;
}

void UHawnmunDungeonSubsystem::EndDungeonSession()
{
	bIsDungeonSession = false;
	ClearSessionData();
}

void UHawnmunDungeonSubsystem::RecordLootItem(const FGameplayTag& ItemTag, int32 Amount)
{
	SessionLootBag.FindOrAdd(ItemTag) += Amount;
	OnSessionLootUpdated.Broadcast();
}

void UHawnmunDungeonSubsystem::SaveCheckpoint(const FTransform& InTransform)
{
	CurrentCheckpoint = InTransform;
	bHasCheckpoint = true;
}

void UHawnmunDungeonSubsystem::MarkDungeonCleared()
{
	if (bDungeonCleared) return;

	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : DungeonStartTimeSeconds;

	ClearTimeSeconds = FMath::Max(0.f, Now - DungeonStartTimeSeconds);
	bDungeonCleared = true;
}

void UHawnmunDungeonSubsystem::ClearSessionData()
{
	CurrentDungeonTag = FGameplayTag();
	DungeonStartTimeSeconds = 0.f;
	ClearTimeSeconds = 0.f;
	bDungeonCleared = false;

	SessionLootBag.Empty();
	bHasCheckpoint = false;

	Debug::Print("Clear Session Data");
}
