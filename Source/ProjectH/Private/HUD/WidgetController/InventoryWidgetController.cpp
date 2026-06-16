// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/InventoryWidgetController.h"
#include "HawnmunGameplayTags.h"
#include "GameMode/HawnmunPlayerState.h"
#include "Components/InventoryComponent.h"
#include "Controllers/HawnmunPlayerController.h"
#include "GameMode/HawnmunGameInstance.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectH/ProjectH.h"

#pragma region Core And Initialization
bool UInventoryWidgetController::CheckIfInDungeon() const
{
	if (auto* DungeonSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		return DungeonSubsystem->IsInDungeonSession();
	}
	return false;
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();
	UInventoryComponent* InvComp = HPS ? HPS->GetInventoryComponent() : nullptr;
	if (!InvComp) return;

	OnMaxSlotsChanged.Broadcast(InvComp->GetMaxSlots());

	if (CheckIfInDungeon())
	{
		auto* DungeonSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UHawnmunDungeonSubsystem>();
		if (!DungeonSubsystem) return;

		int32 SlotIndex = 0;
		for (const auto& Pair : DungeonSubsystem->GetSessionLootBag())
		{
			OnInventorySlotChanged.Broadcast(SlotIndex, Pair.Key);
			OnInventoryStackChanged.Broadcast(SlotIndex, Pair.Value);
			SlotIndex++;
		}

		int32 MaxSlots = InvComp->GetMaxSlots();
		for (int32 i = SlotIndex; i < MaxSlots; ++i)
		{
			OnInventorySlotChanged.Broadcast(i, FGameplayTag());
			OnInventoryStackChanged.Broadcast(i, 0);
		}
	}
	else
	{
		const TArray<FInventorySlotInfo>& Items = InvComp->GetInventoryItems();
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			OnInventorySlotChanged.Broadcast(i, Items[i].ItemTag);
			OnInventoryStackChanged.Broadcast(i, Items[i].Stack);
		}
	}

	BroadcastPlayerInfo();
	HPS->OnXPChangedDelegate.AddWeakLambda(this, [this](int32)
	{
		BroadcastPlayerInfo();
	});

	HPS->OnLevelChangeDelegate.AddWeakLambda(this, [this](int32, bool)
	{
		BroadcastPlayerInfo();
	});
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();
	UInventoryComponent* InvComp = HPS ? HPS->GetInventoryComponent() : nullptr;
	if (!InvComp) return;

	InvComp->OnInventorySlotUpdated.AddWeakLambda(this, [this](int32 SlotIndex, int32 NewStack)
	{
		if (!CheckIfInDungeon())
		{
			OnInventoryStackChanged.Broadcast(SlotIndex, NewStack);
		}
	});

	InvComp->OnInventoryStructureChanged.AddWeakLambda(this, [this](int32 SlotIndex, bool bAdded)
	{
		if (!CheckIfInDungeon())
		{
			const auto& Items = GetHawnmunPS()->GetInventoryComponent()->GetInventoryItems();
			if (Items.IsValidIndex(SlotIndex))
			{
				OnInventorySlotChanged.Broadcast(SlotIndex, Items[SlotIndex].ItemTag);
				OnInventoryStackChanged.Broadcast(SlotIndex, Items[SlotIndex].Stack);
			}
			else
			{
				OnInventorySlotChanged.Broadcast(SlotIndex, FGameplayTag());
				OnInventoryStackChanged.Broadcast(SlotIndex, 0);
			}
		}

		OnInventoryStructureChanged.Broadcast(SlotIndex, bAdded);
	});

	InvComp->OnInventoryChanged.AddWeakLambda(this,[this](const TArray<FInventorySlotInfo>& NewList)
	{
		BroadcastInitialValues();
	});

	if (auto* DungeonSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UHawnmunDungeonSubsystem>())
	{
		DungeonSubsystem->OnSessionLootUpdated.AddWeakLambda(this, [this]()
		{
			if (CheckIfInDungeon())
			{
				BroadcastInitialValues();
			}
		});
	}
}
#pragma endregion

#pragma region Inventory Actions
void UInventoryWidgetController::UseItemRequest()
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();

	if (HPS && SelectedItemTag.IsValid())
	{
		HPS->UseConsumableItem(SelectedItemTag);
		OnSlotSelected(SelectedSlotIndex);
	}
}

void UInventoryWidgetController::EquipItemRequest()
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();

	if (HPS && SelectedItemTag.IsValid())
	{
		HPS->EquipItem(SelectedItemTag);
		OnSlotSelected(SelectedSlotIndex);
	}
}

void UInventoryWidgetController::HandleActionButtonClicked()
{
	if (!SelectedItemTag.IsValid() || SelectedSlotIndex == -1) return;

	switch (ItemAction)
	{
	case EItemActionType::Equip:
		EquipItemRequest();
		break;
	case EItemActionType::Use:
		UseItemRequest();
		break;
	case EItemActionType::None:
	default:
		break;
	}
}

void UInventoryWidgetController::CloseInventoryWidgetByButton()
{
	auto* HPC = GetHawnmunPC();
	HPC->CloseCurrentWidget();
}
#pragma endregion

#pragma region Item Selection And Data
void UInventoryWidgetController::OnSlotSelected(int32 SlotIndex)
{
	if (CheckIfInDungeon())
	{
		auto* DungeonSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UHawnmunDungeonSubsystem>();
		if (!DungeonSubsystem) return;

		int32 CurrentIndex = 0;
		for (const auto& Pair : DungeonSubsystem->GetSessionLootBag())
		{
			if (CurrentIndex == SlotIndex)
			{
				SelectedSlotIndex = SlotIndex;
				SelectedItemTag = Pair.Key;

				FItemBaseRow RowData = GetItemBaseRowByTag(Pair.Key);
				GetActionTypeForItem(Pair.Key);
				OnItemDescriptionUpdate.Broadcast(RowData);
				return;
			}
			CurrentIndex++;
		}
		ResetDescription();
		return;
	}

	AHawnmunPlayerState* HPS = GetHawnmunPS();
	UInventoryComponent* InvComp = HPS ? HPS->GetInventoryComponent() : nullptr;

	if (InvComp && InvComp->GetInventoryItems().IsValidIndex(SlotIndex))
	{
		FGameplayTag SelectedTag = InvComp->GetInventoryItems()[SlotIndex].ItemTag;

		if (SelectedTag.IsValid())
		{
			SelectedSlotIndex = SlotIndex;
			SelectedItemTag = SelectedTag;

			FItemBaseRow RowData = GetItemBaseRowByTag(SelectedTag);
			GetActionTypeForItem(SelectedTag);
			OnItemDescriptionUpdate.Broadcast(RowData);
		}
		else
		{
			ResetDescription();
		}
	}
}

void UInventoryWidgetController::ResetDescription()
{
	SelectedSlotIndex = -1;
	SelectedItemTag = FGameplayTag();
	ItemAction = EItemActionType::None;

	OnItemDescriptionUpdate.Broadcast(FItemBaseRow());
	OnItemActionChanged.Broadcast();
}

void UInventoryWidgetController::GetActionTypeForItem(FGameplayTag ItemTag)
{
	ItemAction = EItemActionType::None;

	if (CheckIfInDungeon())
	{
		OnItemActionChanged.Broadcast();
		return;
	}

	if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Equip))
	{
		ItemAction = EItemActionType::Equip;
	}
	else if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_HP) || ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_Ammo))
	{
		ItemAction = EItemActionType::Equip;
	}
	else if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable))
	{
		ItemAction = EItemActionType::Use;
	}

	OnItemActionChanged.Broadcast();
}

FItemBaseRow UInventoryWidgetController::GetItemBaseRowByTag(FGameplayTag ItemTag) const
{
	UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(this));
	
	if (!GI || !ItemTag.IsValid()) return FItemBaseRow();

	if (FItemBaseRow* FoundRow = GI->GetItemRowByTag(ItemTag))
	{
		return *FoundRow;
	}

	return FItemBaseRow();
}
#pragma endregion

#pragma region Player Info
void UInventoryWidgetController::BroadcastPlayerInfo()
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();
	OnPlayerInfoChanged.Broadcast(HPS->GetPlayerLevel(), HPS->GetPlayerXP(), HPS->GetCurrentLevelRequirement(), HPS->GetNextLevelRequirement());
}
#pragma endregion