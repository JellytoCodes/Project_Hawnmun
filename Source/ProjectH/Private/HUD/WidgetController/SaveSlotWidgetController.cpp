// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/SaveSlotWidgetController.h"

#include "GameMode/HawnmunGameInstance.h"
#include "GameMode/HawnmunGameModeBase.h"
#include "GameMode/HawnmunSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Controllers/HawnmunPlayerController.h"

#pragma region Initialization And State
void USaveSlotWidgetController::SetIsTitleMenu(bool bInIsTitleMenu)
{
	bIsTitleMenu = bInIsTitleMenu;
}
#pragma endregion

#pragma region Data Management
void USaveSlotWidgetController::LoadAllSlotData()
{
	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) return;

	const int32 MaxSlots = 4;

	for (int32 i = 1; i <= MaxSlots; ++i)
	{
		FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), i);
		UHawnmunSaveGame* SaveData = GameMode->GetSaveSlotData(SlotName, i);

		FSaveSlotData DataToBroadcast;
		DataToBroadcast.SlotIndex = i;

		if (SaveData && SaveData->SaveSlotStatus != 0)
		{
			DataToBroadcast.bIsEmpty = false;
			DataToBroadcast.PlayerLevel = SaveData->PlayerLevel;
			
			DataToBroadcast.DisplayLocationName = TEXT("은신처"); 
			
			DataToBroadcast.SaveDate = SaveData->SaveDate;
		}
		else
		{
			DataToBroadcast.bIsEmpty = true;
		}
		
		OnSaveSlotLoaded.Broadcast(DataToBroadcast);
	}
}
#pragma endregion

#pragma region Save Slot Actions
void USaveSlotWidgetController::RequestPlayGame(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

	if (UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->SetCurrentSaveSlot(SlotName, SlotIndex);
	}

	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex))
	{
		UHawnmunSaveGame* SaveData = Cast<UHawnmunSaveGame>(SaveGameObject);
		UGameplayStatics::SaveGameToSlot(SaveData, TEXT("AutoSave_Progress"), 99);

		if (SaveData && SaveData->SaveSlotStatus == Valid)
		{
			UGameplayStatics::OpenLevel(this, FName(*SaveData->LocationName));
			return;
		}
	}

	UClass* ClassToUse = GameMode && GameMode->SaveGameClass ? GameMode->SaveGameClass.Get() : UHawnmunSaveGame::StaticClass();
	if (UHawnmunSaveGame* NewSaveData = Cast<UHawnmunSaveGame>(UGameplayStatics::CreateSaveGameObject(ClassToUse)))
	{
		NewSaveData->bFirstTimeLoadIn = true;
		NewSaveData->SaveSlotStatus = NotValid;
		
		UGameplayStatics::SaveGameToSlot(NewSaveData, SlotName, SlotIndex);
		UGameplayStatics::SaveGameToSlot(NewSaveData, TEXT("AutoSave_Progress"), 99);
	}

	UGameplayStatics::OpenLevel(this, TEXT("FirstStartMap"));
}

void USaveSlotWidgetController::RequestSaveGame(int32 SlotIndex)
{
	if (bIsTitleMenu) return;

	AHawnmunGameModeBase* GameMode = Cast<AHawnmunGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) return;

	FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
	
	GameMode->SaveProgressData();
	GameMode->SyncProgressToSlot(SlotName, SlotIndex);

	LoadAllSlotData(); 
}

void USaveSlotWidgetController::CloseSaveWidgetByButton()
{
	if (bIsTitleMenu) return;

	auto* HPC = GetHawnmunPC();

	HPC->CloseCurrentWidget();
}
#pragma endregion