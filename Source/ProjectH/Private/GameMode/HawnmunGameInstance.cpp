// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "GameMode/HawnmunGameInstance.h"
#include "HawnmunGameplayTags.h"
#include "ProjectH/ProjectH.h"

#pragma region Item Data Management
FItemBaseRow* UHawnmunGameInstance::GetItemRowByTag(const FGameplayTag& ItemTag)
{
	if (!ItemTag.IsValid()) return nullptr;

	const FName RowName = ItemTag.GetTagName();

	if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable))
	{
		if (ConsumableTable)
		{
			return ConsumableTable->FindRow<FConsumableRow>(RowName, TEXT("ConsumableLookup"));
		}
	}
	else if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Equip))
	{
		if (EquipmentTable)
		{
			return EquipmentTable->FindRow<FEquipmentRow>(RowName, TEXT("EquipmentLookup"));
		}
	}

	return nullptr;
}

bool UHawnmunGameInstance::GetConsumableData(FGameplayTag ItemTag, FConsumableRow& OutData)
{
	if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable) && ConsumableTable)
	{
		if (FConsumableRow* Row = ConsumableTable->FindRow<FConsumableRow>(ItemTag.GetTagName(), TEXT("")))
		{
			OutData = *Row;
			return true;
		}
	}
	return false;
}

bool UHawnmunGameInstance::GetEquipmentData(FGameplayTag ItemTag, FEquipmentRow& OutData)
{
	if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Equip) && EquipmentTable)
	{
		if (FEquipmentRow* Row = EquipmentTable->FindRow<FEquipmentRow>(ItemTag.GetTagName(), TEXT("")))
		{
			OutData = *Row;
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Save Data Management
void UHawnmunGameInstance::SetCurrentSaveSlot(const FString& InSlotName, const int32 InSlotIndex)
{
	CurrentSaveSlotName = InSlotName;
	CurrentSaveSlotIndex = InSlotIndex;
}
#pragma endregion

#pragma region FX Data Management
bool UHawnmunGameInstance::GetHitReactData(FGameplayTag HitTag, FHitReactRow& OutData)
{
	if (HitTag.IsValid() && HitReactTable)
	{
		if (FHitReactRow* Row = HitReactTable->FindRow<FHitReactRow>(HitTag.GetTagName(), TEXT("HitReactLookup")))
		{
			OutData = *Row;
			return true;
		}
	}
	return false;
}
#pragma endregion