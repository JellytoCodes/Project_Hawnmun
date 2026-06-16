// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "HawnmunSaveGame.generated.h"

UENUM()
enum ESaveSlotStatus
{
	NotValid,
	Valid
};

UCLASS()
class PROJECTH_API UHawnmunSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
#pragma region Core And Status
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = NotValid;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	bool bFirstTimeLoadIn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	FDateTime SaveDate;
#pragma endregion

#pragma region World And Progression
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	FString LocationName = TEXT("");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Dungeon")
	TMap<FGameplayTag, bool> ClearedDungeons;
#pragma endregion

#pragma region Player Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Player")
	int32 PlayerLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Player")
	int32 PlayerXP = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Player")
	int32 AttributePoints = 0;
#pragma endregion

#pragma region Player Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Strength = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Intelligence = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Resilience = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Vigor = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Health = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Attributes")
	float Stamina = 0.f;
#pragma endregion

#pragma region Inventory And Equipment
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Inventory")
	TMap<FGameplayTag, int32> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData|Equipment")
	TMap<FGameplayTag, FGameplayTag> EquippedItems;
#pragma endregion
};