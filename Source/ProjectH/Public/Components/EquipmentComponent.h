// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UGameplayEffect;

#pragma region Data Structures And Delegates
USTRUCT(BlueprintType)
struct FEquippedSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag ItemTag;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveHandle;

	UPROPERTY()
	int32 Amount;

	FEquippedSlot() : ItemTag(FGameplayTag()), ActiveHandle(FActiveGameplayEffectHandle()) {}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentChangedSignature, FGameplayTag /*SlotTag*/);
#pragma endregion

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTH_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Core And Initialization
public:
	UEquipmentComponent();
#pragma endregion

#pragma region Equipment Actions
public:
	void EquipItem(const FGameplayTag& SlotTag, const FGameplayTag& ItemTag, TSubclassOf<UGameplayEffect> EffectClass, int32 Amount);
	void UnequipItem(const FGameplayTag& SlotTag);
#pragma endregion

#pragma region Equipment Data And Getters
public:
	UFUNCTION(BlueprintPure, Category = "EquipmentComponent")
	TMap<FGameplayTag, FGameplayTag> GetEquipmentData() const;

	FGameplayTag GetEquippedItemTag(const FGameplayTag& SlotTag) const;
	int32 GetEquippedItemBaseDamage(const FGameplayTag& SlotTag) const;
#pragma endregion

#pragma region Delegates And Properties
public:
	FOnEquipmentChangedSignature OnEquipmentChanged;

private:
	UPROPERTY()
	TMap<FGameplayTag, FEquippedSlot> EquippedItems;
#pragma endregion
};