// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HawnmunAbilitySystemComponent.generated.h"

class UHawnmunGameplayAbility;

// 타겟 디버깅 활성화 (Showdebug Abilitsystem 선택한 대상 정보 표시)
// [/Script/GameplayAbilities.AbilitySystemGlobals]
// bUseDebugTargetFromHud=true

UCLASS()
class PROJECTH_API UHawnmunAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region Initialization And Setup
public:
	void AddCharacterActivateAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& ActivateAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& PassiveAbilities);
#pragma endregion

#pragma region Input Handling
public:
	void AbilityInputTagPressed(const FGameplayTag& InInputTag);
	void AbilityInputTagReleased(const FGameplayTag& InInputTag);
#pragma endregion

#pragma region Attribute Management
public:
	void ApplyAttributeUpgrades(const TMap<FGameplayTag, int32>& Upgrades);
#pragma endregion
};