// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "HUD/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/HawnmunAttributeSet.h"
#include "GameMode/HawnmunPlayerState.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "GameMode/HawnmunGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystemGlobals.h"

#pragma region Core And Initialization
void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetHawnmunAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetHawnmunAS()->GetMaxHealth());
	OnStaminaChanged.Broadcast(GetHawnmunAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetHawnmunAS()->GetMaxStamina());

	UpdateEquippedSlotUI(HawnmunGameplayTags::EquipSlot_Weapon);
	UpdateEquippedSlotUI(HawnmunGameplayTags::EquipSlot_Ammo);
	UpdateEquippedSlotUI(HawnmunGameplayTags::EquipSlot_Potion);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetHealthAttribute()).
		AddLambda([&](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetMaxHealthAttribute()).
		AddLambda([&](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetStaminaAttribute()).
		AddLambda([&](const FOnAttributeChangeData& Data) { OnStaminaChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHawnmunAS()->GetMaxStaminaAttribute()).
		AddLambda([&](const FOnAttributeChangeData& Data) { OnMaxStaminaChanged.Broadcast(Data.NewValue); });

	AHawnmunPlayerState* HPS = GetHawnmunPS();
	if (!HPS) return;

	UEquipmentComponent* EquipComp = HPS->GetEquipmentComponent();
	UInventoryComponent* InvComp = HPS->GetInventoryComponent();

	HPS->OnEquippedWeaponChangedDelegate.AddLambda([this](const bool flag)
	{
		OnEquippedWeaponChangedAnimation.Broadcast(flag);
	});

	if (EquipComp)
	{
		EquipComp->OnEquipmentChanged.AddLambda([this](FGameplayTag SlotTag)
		{
			UpdateEquippedSlotUI(SlotTag);
		});
	}

	if (InvComp)
	{
		auto UpdateConsumableSlots = [this]()
		{
			UpdateEquippedSlotUI(HawnmunGameplayTags::EquipSlot_Ammo);
			UpdateEquippedSlotUI(HawnmunGameplayTags::EquipSlot_Potion);
		};

		InvComp->OnInventoryChanged.AddLambda([=](const TArray<FInventorySlotInfo>&) { UpdateConsumableSlots(); });
		InvComp->OnInventorySlotUpdated.AddLambda([=](int32, int32) { UpdateConsumableSlots(); });
		InvComp->OnInventoryStructureChanged.AddLambda([=](int32, bool) { UpdateConsumableSlots(); });
	}

	BindCooldownDelegates();
}
#pragma endregion

#pragma region Equipment And Inventory
void UOverlayWidgetController::UpdateEquippedSlotUI(FGameplayTag SlotTag)
{
	AHawnmunPlayerState* HPS = GetHawnmunPS();
	if (!HPS) return;

	UEquipmentComponent* EquipComp = HPS->GetEquipmentComponent();
	UInventoryComponent* InvComp = HPS->GetInventoryComponent();
	UHawnmunGameInstance* GI = Cast<UHawnmunGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (!EquipComp || !InvComp || !GI) return;

	FGameplayTag EquippedItemTag = EquipComp->GetEquippedItemTag(SlotTag);

	if (!EquippedItemTag.IsValid())
	{
		OnEquippedSlotUpdated.Broadcast(SlotTag, nullptr, 0);
		return;
	}

	UTexture2D* ItemIcon = nullptr;
	if (FItemBaseRow* ItemData = GI->GetItemRowByTag(EquippedItemTag))
	{
		ItemIcon = ItemData->Icon;
	}

	int32 ItemCount = 1;
	if (!SlotTag.MatchesTagExact(HawnmunGameplayTags::EquipSlot_Weapon))
	{
		ItemCount = InvComp->GetItemCount(EquippedItemTag);
	}

	OnEquippedSlotUpdated.Broadcast(SlotTag, ItemIcon, ItemCount);
}
#pragma endregion

#pragma region Boss UI
void UOverlayWidgetController::BindBossAttributes(AActor* BossActor, const FString& BossName)
{
	UAbilitySystemComponent* BossASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(BossActor);
	const UHawnmunAttributeSet* BossAS = Cast<UHawnmunAttributeSet>(BossASC->GetAttributeSet(UHawnmunAttributeSet::StaticClass()));

	OnBossInfoSet.Broadcast(BossName);

	OnBossMaxHealthChanged.Broadcast(BossAS->GetMaxHealth());
	OnBossHealthChanged.Broadcast(BossAS->GetHealth());

	BossASC->GetGameplayAttributeValueChangeDelegate(BossAS->GetHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnBossHealthChanged.Broadcast(Data.NewValue);
		});

	BossASC->GetGameplayAttributeValueChangeDelegate(BossAS->GetMaxHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnBossMaxHealthChanged.Broadcast(Data.NewValue);
		});
}

void UOverlayWidgetController::ClearBossAttributes()
{
	OnBossHealthChanged.Broadcast(0.f);
	OnBossMaxHealthChanged.Broadcast(1.f);
	OnBossInfoCleared.Broadcast();
}
#pragma endregion

#pragma region Cooldown UI
void UOverlayWidgetController::BindCooldownDelegates()
{
    UHawnmunAbilitySystemComponent* HawnmunASC = GetHawnmunASC();
    if (!HawnmunASC) return;

    HawnmunASC->OnActiveGameplayEffectAddedDelegateToSelf.AddLambda(
        [this, HawnmunASC](UAbilitySystemComponent*, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
        {
            FGameplayTagContainer GrantedTags;
            Spec.GetAllGrantedTags(GrantedTags);

            for (const FGameplayTag& CooldownTag : TrackedCooldownTags)
            {
                if (GrantedTags.HasTagExact(CooldownTag))
                {
                    float Duration = Spec.GetDuration();
                    BroadcastCooldownUpdate(CooldownTag, Duration, Duration);

                    ActiveCooldownHandles.Add(CooldownTag, Handle);
                }
            }
        });

    HawnmunASC->OnAnyGameplayEffectRemovedDelegate().AddLambda(
        [this](const FActiveGameplayEffect& RemovedEffect)
        {
            FGameplayTagContainer GrantedTags;
            RemovedEffect.Spec.GetAllGrantedTags(GrantedTags);

            for (const FGameplayTag& CooldownTag : TrackedCooldownTags)
            {
                if (GrantedTags.HasTagExact(CooldownTag))
                {
                    BroadcastCooldownUpdate(CooldownTag, 0.f, 0.f);
                    ActiveCooldownHandles.Remove(CooldownTag);
                }
            }
        });
}

void UOverlayWidgetController::BroadcastCooldownUpdate(FGameplayTag CooldownTag, float TimeRemaining, float Duration)
{
    OnCooldownChanged.Broadcast(CooldownTag, TimeRemaining, Duration);
}
#pragma endregion