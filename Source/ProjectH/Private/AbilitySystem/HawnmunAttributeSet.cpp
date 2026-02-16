// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/HawnmunAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "HawnmunGameplayTags.h"

UHawnmunAttributeSet::UHawnmunAttributeSet()
{
	InitStamina(100.f);
	InitStaminaRegeneration(1.f);
	InitHealthRegeneration(1.f);
}

void UHawnmunAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UHawnmunAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (!GetOwningAbilitySystemComponent()) return;

	const bool bStaminaChanged = (Data.EvaluatedData.Attribute == GetStaminaAttribute()) || (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute());

	if (bStaminaChanged)
	{
		const float MaxSta = GetMaxStamina();
		const float Sta = GetStamina();
		const bool bShouldBeFull = (Sta >= MaxSta - 0.01f);

		const FGameplayTag FullTag = HawnmunGameplayTags::State_Stamina_Full;
		const bool bIsFullNow = GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(FullTag);

		if (bShouldBeFull && !bIsFullNow)
		{
			GetOwningAbilitySystemComponent()->AddLooseGameplayTag(FullTag);
		}
		else if (!bShouldBeFull && bIsFullNow)
		{
			GetOwningAbilitySystemComponent()->RemoveLooseGameplayTag(FullTag);
		}
	}
}
