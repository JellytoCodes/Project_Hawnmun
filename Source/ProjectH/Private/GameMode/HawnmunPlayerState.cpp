// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "GameMode/HawnmunPlayerState.h"
#include "GameMode/HawnmunSaveGame.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "GameMode/HawnmunGameInstance.h"
#include "Types/HawnmunStructTypes.h"
#include "HawnmunGameplayTags.h"
#include "GameMode/Subsystem/HawnmunDungeonSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectH/ProjectH.h"

#pragma region Core And Components
AHawnmunPlayerState::AHawnmunPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHawnmunAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UHawnmunAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
}
#pragma endregion

#pragma region Player Progression
void AHawnmunPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

int32 AHawnmunPlayerState::GetNextLevelRequirement() const
{
	if (!LevelUpDataTable)
	{
		return 0;
	}

	const int32 MaxLevel = LevelUpDataTable->GetRowMap().Num();
	if (Level >= MaxLevel)
	{
		return 0;
	}

	const FString NextLevelString = FString::FromInt(Level + 1);
	const FLevelUpInfoRow* NextLevelRow = LevelUpDataTable->FindRow<FLevelUpInfoRow>(
		FName(*NextLevelString),
		TEXT("Find Next Level Info")
	);

	return NextLevelRow ? NextLevelRow->LevelUpRequirement : 0;
}

int32 AHawnmunPlayerState::GetCurrentLevelRequirement() const
{
	if (!LevelUpDataTable || Level <= 1)
	{
		return 0;
	}

	const FString CurrentLevelString = FString::FromInt(Level);
	const FLevelUpInfoRow* CurrentLevelRow = LevelUpDataTable->FindRow<FLevelUpInfoRow>(
		FName(*CurrentLevelString),
		TEXT("Find Current Level Info")
	);

	return CurrentLevelRow ? CurrentLevelRow->LevelUpRequirement : 0;
}

void AHawnmunPlayerState::AddToXP(int32 InXP)
{
	if (!LevelUpDataTable) return;

	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);

	bool bLeveledUp = false;
	int32 GainedAttributePoints = 0;

	const int32 MaxLevel = LevelUpDataTable->GetRowMap().Num();

	while (Level < MaxLevel)
	{
		FString NextLevelString = FString::FromInt(Level + 1);
		FLevelUpInfoRow* NextLevelRow = LevelUpDataTable->FindRow<FLevelUpInfoRow>(FName(*NextLevelString), TEXT("Find Next Level Info"));

		if (!NextLevelRow) break;

		if (XP >= NextLevelRow->LevelUpRequirement)
		{
			Level++;
			GainedAttributePoints += NextLevelRow->AttributePointAward;
			bLeveledUp = true;
		}
		else
		{
			break;
		}
	}

	if (bLeveledUp)
	{
		AddToAttributePoints(GainedAttributePoints);
		OnLevelChangeDelegate.Broadcast(Level, true);
	}
}
#pragma endregion

#pragma region Inventory And Equipment
bool AHawnmunPlayerState::UseConsumableItem(FGameplayTag ItemTag)
{
	if (!InventoryComponent || !AbilitySystemComponent) return false;

	UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI) return false;

	FConsumableRow ConsumableData;
	if (GI->GetConsumableData(ItemTag, ConsumableData))
	{
		if (InventoryComponent->RemoveItem(ItemTag))
		{
			if (ConsumableData.Amount > 0)
			{
				if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_XP))
				{
					AddToXP(ConsumableData.Amount);
				}
				else if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_Gold))
				{
					// 미구현
				}
				// Ammo는 GA에서 데미지 연산에 사용되기 때문에 별도 로직 없음

				return true;
			}

			if (ConsumableData.ConsumableEffect)
			{
				FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
				ContextHandle.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ConsumableData.ConsumableEffect, 1.0f, ContextHandle);
				if (SpecHandle.IsValid())
				{
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}

				return true;
			}
		}
	}

	return false;
}

void AHawnmunPlayerState::EquipItem(FGameplayTag ItemTag)
{
	if (!EquipmentComponent) return;

	UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI) return;

	FEquipmentRow EquipData;
	if (GI->GetEquipmentData(ItemTag, EquipData))
	{
		EquipmentComponent->EquipItem(EquipData.EquipSlotTag, ItemTag, EquipData.EquipmentEffect, EquipData.Amount);
		return;
	}

	FConsumableRow ConsumableData;
	if (GI->GetConsumableData(ItemTag, ConsumableData))
	{
		if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_HP))
			EquipmentComponent->EquipItem(HawnmunGameplayTags::EquipSlot_Potion, ItemTag, nullptr, 0);
		else if (ItemTag.MatchesTag(HawnmunGameplayTags::Item_Consumable_Ammo))
			EquipmentComponent->EquipItem(HawnmunGameplayTags::EquipSlot_Ammo, ItemTag, nullptr, ConsumableData.Amount);
	}
}

bool AHawnmunPlayerState::UseEquippedPotion()
{
	return UseConsumableItem(EquipmentComponent->GetEquippedItemTag(HawnmunGameplayTags::EquipSlot_Potion));
}

bool AHawnmunPlayerState::UseEquippedAmmo()
{
	return UseConsumableItem(EquipmentComponent->GetEquippedItemTag(HawnmunGameplayTags::EquipSlot_Ammo));
}

void AHawnmunPlayerState::EquippedWeaponChanged(const bool bInFlag)
{
	if (bIsEquippedMainWeapon == bInFlag) return;

	bIsEquippedMainWeapon = bInFlag;
	OnEquippedWeaponChangedDelegate.Broadcast(bIsEquippedMainWeapon);
}

void AHawnmunPlayerState::CommitDungeonLoot()
{
	auto* DungeonSubsystem = GetGameInstance()->GetSubsystem<UHawnmunDungeonSubsystem>();
	if (!DungeonSubsystem) return;

	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		for (const auto& Loot : DungeonSubsystem->GetSessionLootBag())
		{
			InvComp->AddItem(Loot.Key, Loot.Value);
		}
	}
	DungeonSubsystem->EndDungeonSession();
}
#pragma endregion

#pragma region Save And Load
void AHawnmunPlayerState::SaveInventoryAndEquipment(UHawnmunSaveGame* SaveGame)
{
	if (!SaveGame || !InventoryComponent || !EquipmentComponent) return;

	SaveGame->InventoryItems = InventoryComponent->GetInventoryData();
	SaveGame->EquippedItems = EquipmentComponent->GetEquipmentData();
}

void AHawnmunPlayerState::LoadInventoryAndEquipment(UHawnmunSaveGame* SaveGame)
{
	if (!SaveGame || !InventoryComponent || !EquipmentComponent) return;

	InventoryComponent->LoadInventoryData(SaveGame->InventoryItems);

	for (const auto& Pair : SaveGame->EquippedItems)
	{
		if (Pair.Value.IsValid())
		{
			EquipItem(Pair.Value);
		}
	}
}
#pragma endregion