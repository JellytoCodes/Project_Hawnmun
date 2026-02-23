// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/HawnmunAbilitySystemGlobals.h"
#include "AbilitySystem/HawnmunGameplayEffectContext.h"

FGameplayEffectContext* UHawnmunAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FHawnmunGameplayEffectContext();
}
