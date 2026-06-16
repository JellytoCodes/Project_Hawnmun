// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Components/EquipmentComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "HawnmunGameplayTags.h"

#pragma region Core And Initialization
UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EquippedItems.Add(HawnmunGameplayTags::EquipSlot_Weapon, FEquippedSlot());
	EquippedItems.Add(HawnmunGameplayTags::EquipSlot_Ammo, FEquippedSlot());
	EquippedItems.Add(HawnmunGameplayTags::EquipSlot_Potion, FEquippedSlot());
}
#pragma endregion

#pragma region Equipment Actions
void UEquipmentComponent::EquipItem(const FGameplayTag& SlotTag, const FGameplayTag& ItemTag, TSubclassOf<UGameplayEffect> EffectClass, int32 Amount)
{
	FEquippedSlot* TargetSlot = EquippedItems.Find(SlotTag);
	if (!TargetSlot) return;

	if (TargetSlot->ItemTag.IsValid())
	{
		UnequipItem(SlotTag);
	}

	TargetSlot->ItemTag = ItemTag;
	TargetSlot->Amount = Amount;

	if (EffectClass)
	{
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
				ContextHandle.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, ContextHandle);
				if (SpecHandle.IsValid())
				{
					TargetSlot->ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}

	OnEquipmentChanged.Broadcast(SlotTag);
}

void UEquipmentComponent::UnequipItem(const FGameplayTag& SlotTag)
{
	FEquippedSlot* TargetSlot = EquippedItems.Find(SlotTag);
	if (!TargetSlot) return;

	if (TargetSlot->ActiveHandle.IsValid())
	{
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
			{
				ASC->RemoveActiveGameplayEffect(TargetSlot->ActiveHandle);
			}
		}
	}

	TargetSlot->ItemTag = FGameplayTag();
	TargetSlot->Amount = 0;
	TargetSlot->ActiveHandle = FActiveGameplayEffectHandle();

	OnEquipmentChanged.Broadcast(SlotTag);
}
#pragma endregion

#pragma region Equipment Data And Getters
TMap<FGameplayTag, FGameplayTag> UEquipmentComponent::GetEquipmentData() const
{
	TMap<FGameplayTag, FGameplayTag> Data;
	for (const auto& Pair : EquippedItems)
	{
		Data.Add(Pair.Key, Pair.Value.ItemTag);
	}
	return Data;
}

FGameplayTag UEquipmentComponent::GetEquippedItemTag(const FGameplayTag& SlotTag) const
{
	if (const FEquippedSlot* Slot = EquippedItems.Find(SlotTag))
	{
		return Slot->ItemTag;
	}
	return FGameplayTag();
}

int32 UEquipmentComponent::GetEquippedItemBaseDamage(const FGameplayTag& SlotTag) const
{
	if (const FEquippedSlot* Slot = EquippedItems.Find(SlotTag))
	{
		return Slot->Amount;
	}
	return 0;
}
#pragma endregion