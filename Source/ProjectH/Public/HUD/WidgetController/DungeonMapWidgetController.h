// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "GameplayTagContainer.h"
#include "Types/HawnmunStructTypes.h"
#include "DungeonMapWidgetController.generated.h"

#pragma region Data Structures And Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonSelectedSignature, const FDungeonInfoRow&, DungeonInfo);
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UDungeonMapWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

#pragma region Dungeon Actions
public:
	UFUNCTION(BlueprintCallable, Category = "DungeonMap")
	void SelectDungeon(FGameplayTag DungeonTag);

	UFUNCTION(BlueprintCallable, Category = "DungeonMap")
	void EnterSelectedDungeon();

	UFUNCTION(BlueprintCallable, Category = "DungeonMap")
	void CancelSelection();

	UFUNCTION(BlueprintCallable, Category = "DungeonMap")
	void CloseDungeonMapWidgetByButton();
#pragma endregion

#pragma region Widget Delegates And Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "DungeonMap|Events")
	FOnDungeonSelectedSignature OnDungeonSelected;

private:
	UPROPERTY()
	FGameplayTag SelectedDungeonTag;

	UPROPERTY(EditDefaultsOnly, Category = "DungeonMap")
	TObjectPtr<UDataTable> DungeonDataTable;
#pragma endregion
};