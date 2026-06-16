// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Types/HawnmunStructTypes.h"
#include "GameplayTagContainer.h"
#include "HawnmunGameInstance.generated.h"

UCLASS()
class PROJECTH_API UHawnmunGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
#pragma region Item Data Management
public:
	FItemBaseRow* GetItemRowByTag(const FGameplayTag& ItemTag);

	UFUNCTION(BlueprintCallable, Category = "GameData|Item")
	bool GetConsumableData(FGameplayTag ItemTag, FConsumableRow& OutData);

	UFUNCTION(BlueprintCallable, Category = "GameData|Item")
	bool GetEquipmentData(FGameplayTag ItemTag, FEquipmentRow& OutData);

	int32 GetItemMaxStack() const { return ItemMaxStack; }
	void SetItemMaxStack(const int32 InMaxStack) { ItemMaxStack = InMaxStack; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ConsumableTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> EquipmentTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|Item", meta = (AllowPrivateAccess = "true"))
	int32 ItemMaxStack = 99;
#pragma endregion

#pragma region Save Data Management
public:
	FString GetCurrentSaveSlotName() const { return CurrentSaveSlotName; }
	int32 GetCurrentSaveSlotIndex() const { return CurrentSaveSlotIndex; }
	void SetCurrentSaveSlot(const FString& InSlotName, const int32 InSlotIndex);

private:
	FString CurrentSaveSlotName = TEXT("");
	int32 CurrentSaveSlotIndex = -1;
#pragma endregion

#pragma region FX Data Management
public:
	UFUNCTION(BlueprintCallable, Category = "GameData|FX")
	bool GetHitReactData(FGameplayTag HitTag, FHitReactRow& OutData);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|FX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> HitReactTable;
#pragma endregion
};