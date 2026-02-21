// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "ProjectH/ProjectH.h"

void UDataAsset_StartUpDataBase::InitializeGameplayEffect(UAbilitySystemComponent* InASCToGive, EStartUpCharacterName StartUpCharacterName, int32 ApplyLevel)
{
	check(InASCToGive);

	if (!StartUpAttributeInfo.IsEmpty())
	{
		for (const FHawnmunStartUpAttributeInfo& AttributeInfo : StartUpAttributeInfo)
		{
			if (!AttributeInfo.PrimaryGameplayEffect || !AttributeInfo.SecondaryGameplayEffect || !AttributeInfo.VitalGameplayEffect) continue;

			if (AttributeInfo.StartUpCharacterName == StartUpCharacterName)
			{
				FGameplayEffectContextHandle ContextHandle = InASCToGive->MakeEffectContext();
				ContextHandle.AddSourceObject(InASCToGive->GetOwnerActor());

				{ // PrimaryGameplayEffect
					const FGameplayEffectSpecHandle SpecHandle = InASCToGive->MakeOutgoingSpec(AttributeInfo.PrimaryGameplayEffect, ApplyLevel, ContextHandle);
					InASCToGive->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), InASCToGive);				
				}
				{ // SecondaryGameplayEffect
					const FGameplayEffectSpecHandle SpecHandle = InASCToGive->MakeOutgoingSpec(AttributeInfo.SecondaryGameplayEffect, ApplyLevel, ContextHandle);
					Debug::Print(AttributeInfo.SecondaryGameplayEffect->GetPathName());
					InASCToGive->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), InASCToGive);				
				}
				{ // VitalGameplayEffect
					const FGameplayEffectSpecHandle SpecHandle = InASCToGive->MakeOutgoingSpec(AttributeInfo.VitalGameplayEffect, ApplyLevel, ContextHandle);
					InASCToGive->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), InASCToGive);				
				}
			}
		}
	}
}
