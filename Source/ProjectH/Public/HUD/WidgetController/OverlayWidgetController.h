// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "HUD/WidgetController/HawnmunWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

#pragma region Data Structures And Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttribueChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedAnimation, const bool, flag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquippedSlotUpdatedSignature, FGameplayTag, SlotTag, UTexture2D*, Icon, int32, Count);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossStatChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossInfoSetSignature, const FString&, BossName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossInfoClearedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangedSignature, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API UOverlayWidgetController : public UHawnmunWidgetController
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
#pragma endregion

#pragma region Player Attributes
public:
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttribueChangedSignature OnMaxStaminaChanged;
#pragma endregion

#pragma region Equipment And Inventory
public:
	UPROPERTY(BlueprintAssignable, Category = "EquipSlot")
	FOnEquippedSlotUpdatedSignature OnEquippedSlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "EquipSlot")
	FOnEquippedWeaponChangedAnimation OnEquippedWeaponChangedAnimation;

private:
	void UpdateEquippedSlotUI(FGameplayTag SlotTag);
#pragma endregion

#pragma region Boss UI
public:
	UFUNCTION(BlueprintCallable, Category = "GAS|Boss")
	void BindBossAttributes(AActor* BossActor, const FString& BossName);

	UFUNCTION(BlueprintCallable, Category = "GAS|Boss")
	void ClearBossAttributes();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Boss")
	FOnBossStatChangedSignature OnBossHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Boss")
	FOnBossStatChangedSignature OnBossMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Boss")
	FOnBossInfoSetSignature OnBossInfoSet;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Boss")
	FOnBossInfoClearedSignature OnBossInfoCleared;
#pragma endregion

#pragma region Cooldown UI
public:
    UPROPERTY(BlueprintAssignable, Category = "GAS|Cooldown")
    FOnCooldownChangedSignature OnCooldownChanged;

private:
    void BindCooldownDelegates();
    void BroadcastCooldownUpdate(FGameplayTag CooldownTag, float TimeRemaining, float Duration);

    UPROPERTY(EditDefaultsOnly, Category = "GAS|Cooldown")
    TArray<FGameplayTag> TrackedCooldownTags;

    TMap<FGameplayTag, FActiveGameplayEffectHandle> ActiveCooldownHandles;
#pragma endregion
};