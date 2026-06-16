// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "SaveSlotWidgetController.generated.h"

#pragma region Data Structures And Delegates
USTRUCT(BlueprintType)
struct FSaveSlotData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsEmpty = true; 

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerLevel = 1;

	UPROPERTY(BlueprintReadOnly)
	FString LocationName = TEXT("");

	UPROPERTY(BlueprintReadOnly)
	FString DisplayLocationName = TEXT("");

	UPROPERTY(BlueprintReadOnly)
	FDateTime SaveDate;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotLoadedSignature, const FSaveSlotData&, SlotData);
#pragma endregion


UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API USaveSlotWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

#pragma region Initialization And State
public:
	UFUNCTION(BlueprintCallable, Category = "SaveSlot|Init")
	void SetIsTitleMenu(bool bInIsTitleMenu);

	UFUNCTION(BlueprintPure, Category = "SaveSlot|Init")
	bool GetIsTitleMenu() const { return bIsTitleMenu; }

private:
	UPROPERTY()
	bool bIsTitleMenu = false;
#pragma endregion

#pragma region Data Management
public:
	UFUNCTION(BlueprintCallable, Category = "SaveSlot|Data")
	void LoadAllSlotData();

	UPROPERTY(BlueprintAssignable, Category = "SaveSlot|Events")
	FOnSaveSlotLoadedSignature OnSaveSlotLoaded;
#pragma endregion

#pragma region Save Slot Actions
public:
	UFUNCTION(BlueprintCallable, Category = "SaveSlot|Action")
	void RequestPlayGame(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveSlot|Action")
	void RequestSaveGame(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveSlot|Action")
	void CloseSaveWidgetByButton();
#pragma endregion
};