// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

#pragma region Data Structures And Delegates
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGameplayTag ItemTag;

	UPROPERTY()
	int32 Stack;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, const TArray<FInventorySlotInfo>& /*NewItemList*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotUpdatedSignature, int32 /*SlotIndex*/, int32 /*NewStack*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryStructureChangedSignature, int32 /*SlotIndex*/, bool /*bIsAdded*/);
#pragma endregion

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void ClearInventory();

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void LoadInventoryData(const TMap<FGameplayTag, int32>& InventoryData);
#pragma endregion

#pragma region Inventory Actions
public:
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	bool AddItem(FGameplayTag ItemTag, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	bool RemoveItem(FGameplayTag ItemTag, int32 Quantity = 1);
#pragma endregion

#pragma region Inventory Data And Getters
public:
	UFUNCTION(BlueprintPure, Category = "InventoryComponent")
	int32 GetItemCount(const FGameplayTag& ItemTag) const;

	UFUNCTION(BlueprintPure, Category = "InventoryComponent")
	TArray<FInventorySlotInfo> GetInventoryItems() const { return InventoryItems; }

	UFUNCTION(BlueprintPure, Category = "InventoryComponent")
	TMap<FGameplayTag, int32> GetInventoryData() const;

	int32 GetMaxSlots() const { return MaxSlots; }
#pragma endregion

#pragma region Delegates And Properties
public:
	FOnInventoryChangedSignature OnInventoryChanged;
	FOnInventorySlotUpdatedSignature OnInventorySlotUpdated;
	FOnInventoryStructureChangedSignature OnInventoryStructureChanged;

private:
	UPROPERTY()
	int32 MaxSlots = 15;

	UPROPERTY()
	TArray<FInventorySlotInfo> InventoryItems;
#pragma endregion
};