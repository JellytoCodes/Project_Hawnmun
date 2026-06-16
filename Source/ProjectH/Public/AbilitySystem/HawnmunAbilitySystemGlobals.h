// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "HawnmunAbilitySystemGlobals.generated.h"

// GAS 글로벌 데이터 설정
// [/Script/GameplayAbilities.AbilitySystemGlobals]
// AbilitySystemGlobalsClassName="/Script/GameplayAbilities.AbilitySystemGlobals"

UCLASS()
class PROJECTH_API UHawnmunAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
