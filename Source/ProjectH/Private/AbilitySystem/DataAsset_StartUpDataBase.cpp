// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::InitializeGameplayEffect(UAbilitySystemComponent* InASCToGive, EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel)
{
	check(InASCToGive);

	if (!StartUpAttributeInfo.IsEmpty())
	{
		for (const FHawnmunStartUpAttributeInfo& AttributeInfo : StartUpAttributeInfo)
		{
			if (!AttributeInfo.StartUpGameplayEffect || AttributeInfo.StartUpCharacterName != StartUpCharacterName) continue;
			UGameplayEffect* EffectCDO = AttributeInfo.StartUpGameplayEffect->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}