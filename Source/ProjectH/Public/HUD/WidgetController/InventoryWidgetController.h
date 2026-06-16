// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "Types/HawnmunEnumTypes.h"
#include "Types/HawnmunStructTypes.h"
#include "InventoryWidgetController.generated.h"

#pragma region Data Structures And Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotSignature, int32, SlotIndex, FGameplayTag, ItemTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryStackSignature, int32, SlotIndex, int32, NewStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryStructureSignature, int32, SlotIndex, bool, bIsAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxSlotsChangedSignature, int32, NewMaxSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDescriptionSignature, const FItemBaseRow&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemActionChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPlayerInfoChangedSignature, int32, Level, int32, XP, int32, CurrentLevelRequirement, int32, NextLevelRequirement);
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UInventoryWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()
	
#pragma region Core And Initialization
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:
	UFUNCTION(BlueprintPure)
	bool CheckIfInDungeon() const;
#pragma endregion

#pragma region Inventory Actions
public:
	UFUNCTION(Category = "Inventory")
	void UseItemRequest();

	UFUNCTION(Category = "Inventory")
	void EquipItemRequest();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HandleActionButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventoryWidgetByButton();
#pragma endregion

#pragma region Item Selection And Data
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnSlotSelected(int32 SlotIndex);
	
	void ResetDescription();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetActionTypeForItem(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemBaseRow GetItemBaseRowByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	EItemActionType GetItemAction() const { return ItemAction; }
#pragma endregion

#pragma region Widget Delegates And Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnInventorySlotSignature OnInventorySlotChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnInventoryStackSignature OnInventoryStackChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnInventoryStructureSignature OnInventoryStructureChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnMaxSlotsChangedSignature OnMaxSlotsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnItemDescriptionSignature OnItemDescriptionUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
	FOnItemActionChangedSignature OnItemActionChanged;

private:
	EItemActionType ItemAction = EItemActionType::None;

	UPROPERTY()
	int32 SelectedSlotIndex = -1;

	UPROPERTY()
	FGameplayTag SelectedItemTag;
#pragma endregion
	
#pragma region Player Info
	// 인벤토리에서 플레이어 레벨, 경험치를 제공하는 구조를 가져가기 때문에 현재는 이곳에 데이터를 전달할 수 있도록 설계
public :
	UPROPERTY(BlueprintAssignable, Category = "Inventory|PlayerInfo")
	FOnPlayerInfoChangedSignature OnPlayerInfoChanged;

private:
	void BroadcastPlayerInfo();
#pragma endregion
};