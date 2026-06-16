// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/DungeonMapWidgetController.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Controllers/HawnmunPlayerController.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"

#pragma region Dungeon Actions
void UDungeonMapWidgetController::SelectDungeon(FGameplayTag DungeonTag)
{
	if (!DungeonTag.IsValid() || !DungeonDataTable) return;

	FDungeonInfoRow* Row = DungeonDataTable->FindRow<FDungeonInfoRow>(DungeonTag.GetTagName(), TEXT("Find Dungeon Info"));

	if (Row)
	{
		SelectedDungeonTag = DungeonTag;
		OnDungeonSelected.Broadcast(*Row);
	}
}

void UDungeonMapWidgetController::EnterSelectedDungeon()
{
	if (!SelectedDungeonTag.IsValid() || !DungeonDataTable) return;

	FDungeonInfoRow* Row = DungeonDataTable->FindRow<FDungeonInfoRow>(SelectedDungeonTag.GetTagName(), TEXT("Find Dungeon Info"));

	if (Row && Row->TargetLevel.ToSoftObjectPath().IsValid())
	{
		if (AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->SaveProgressData();
		}

		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
		{
			if (UHawnmunDungeonSubsystem* DungeonSubsystem = GI->GetSubsystem<UHawnmunDungeonSubsystem>())
			{
				DungeonSubsystem->StartDungeonSession(SelectedDungeonTag);
			}
		}

		FName LevelName = FName(*Row->TargetLevel.GetAssetName());
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

void UDungeonMapWidgetController::CancelSelection()
{
	SelectedDungeonTag = FGameplayTag();
}

void UDungeonMapWidgetController::CloseDungeonMapWidgetByButton()
{
	auto* HPC = GetHawnmunPC();
	HPC->CloseCurrentWidget();
}
#pragma endregion
