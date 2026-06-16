// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Components/InventoryComponent.h"
#include "HawnmunGameplayTags.h"
#include "GameMode/HawnmunGameInstance.h"
#include "Kismet/GameplayStatics.h"

#pragma region Core And Initialization
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::ClearInventory()
{
	InventoryItems.Empty();
	OnInventoryChanged.Broadcast(InventoryItems);
}

void UInventoryComponent::LoadInventoryData(const TMap<FGameplayTag, int32>& InventoryData)
{
	ClearInventory();
	for (const auto& Pair : InventoryData)
	{
		AddItem(Pair.Key, Pair.Value);
	}
}
#pragma endregion

#pragma region Inventory Actions
bool UInventoryComponent::AddItem(FGameplayTag ItemTag, int32 Quantity)
{
	if (!ItemTag.IsValid() || Quantity <= 0) return false;

	UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI) return false;

	bool bIsEquipment = ItemTag.MatchesTag(HawnmunGameplayTags::Item_Equip);
	int32 RemainingQuantity = Quantity;
	
	const int32 MaxStackLimit = bIsEquipment ? 1 : GI->GetItemMaxStack();

	if (!bIsEquipment)
	{
		for (int32 i = 0; i < InventoryItems.Num() && RemainingQuantity > 0; ++i)
		{
			if (InventoryItems[i].ItemTag == ItemTag && InventoryItems[i].Stack < MaxStackLimit)
			{
				int32 SpaceLeft = MaxStackLimit - InventoryItems[i].Stack;
				int32 AmountToAdd = FMath::Min(SpaceLeft, RemainingQuantity);

				InventoryItems[i].Stack += AmountToAdd;
				RemainingQuantity -= AmountToAdd;

				OnInventorySlotUpdated.Broadcast(i, InventoryItems[i].Stack);
			}
		}
	}

	while (RemainingQuantity > 0)
	{
		if (InventoryItems.Num() >= MaxSlots)
		{
			return false;
		}

		int32 AmountToAdd = FMath::Min(MaxStackLimit, RemainingQuantity);

		FInventorySlotInfo NewSlot;
		NewSlot.ItemTag = ItemTag;
		NewSlot.Stack = AmountToAdd;

		int32 AddedIndex = InventoryItems.Add(NewSlot);
		OnInventoryStructureChanged.Broadcast(AddedIndex, true);

		RemainingQuantity -= AmountToAdd;
	}

	return true;
}

bool UInventoryComponent::RemoveItem(FGameplayTag ItemTag, int32 Quantity)
{
	if (!ItemTag.IsValid() || Quantity <= 0) return false;

	int32 RemainingQuantity = Quantity;

	for (int32 i = InventoryItems.Num() - 1; i >= 0 && RemainingQuantity > 0; --i)
	{
		if (InventoryItems[i].ItemTag == ItemTag)
		{
			if (InventoryItems[i].Stack > RemainingQuantity)
			{
				InventoryItems[i].Stack -= RemainingQuantity;
				OnInventorySlotUpdated.Broadcast(i, InventoryItems[i].Stack);
				RemainingQuantity = 0;
			}
			else
			{
				RemainingQuantity -= InventoryItems[i].Stack;
				InventoryItems.RemoveAt(i);
				OnInventoryStructureChanged.Broadcast(i, false);
			}
		}
	}
	return RemainingQuantity == 0; 
}
#pragma endregion

#pragma region Inventory Data And Getters
TMap<FGameplayTag, int32> UInventoryComponent::GetInventoryData() const
{
	TMap<FGameplayTag, int32> InvData;
	for (const FInventorySlotInfo& Slot : InventoryItems)
	{
		if (Slot.ItemTag.IsValid())
		{
			InvData.FindOrAdd(Slot.ItemTag) += Slot.Stack;
		}
	}
	return InvData;
}

int32 UInventoryComponent::GetItemCount(const FGameplayTag& ItemTag) const
{
	int32 TotalCount = 0;
	for (const FInventorySlotInfo& Slot : InventoryItems)
	{
		if (Slot.ItemTag == ItemTag)
		{
			TotalCount += Slot.Stack;
		}
	}
	return TotalCount;
}
#pragma endregion