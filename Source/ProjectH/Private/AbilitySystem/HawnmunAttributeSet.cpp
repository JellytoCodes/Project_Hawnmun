// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/HawnmunAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "HawnmunGameplayTags.h"
#include "Interfaces/CombatInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

UHawnmunAttributeSet::UHawnmunAttributeSet()
{
	/** Primary Attributes */
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Primary_Vigor, GetVigorAttribute);
	
	/** Secondary Attributes */
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_StaminaRegeneration, GetStaminaRegenerationAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);

	/**Resistance Attributes */
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(HawnmunGameplayTags::Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UHawnmunAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UHawnmunAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	//if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		HandleStamina(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UHawnmunAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

}

void UHawnmunAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{

}

void UHawnmunAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{

}

void UHawnmunAttributeSet::HandleStamina(const FEffectProperties& Props)
{
	const float MaxSta = GetMaxStamina();
	const float Sta = GetStamina();
	const bool bShouldBeFull = (Sta >= MaxSta - 0.01f);

	const FGameplayTag FullTag = HawnmunGameplayTags::State_Stamina_Full;
	const bool bIsFullNow = Props.SourceASC->HasMatchingGameplayTag(FullTag);

	if (bShouldBeFull && !bIsFullNow)
	{
		Props.SourceASC->AddLooseGameplayTag(FullTag);
	}
	else if (!bShouldBeFull && bIsFullNow)
	{
		Props.SourceASC->RemoveLooseGameplayTag(FullTag);
	}
}

void UHawnmunAttributeSet::Debuff(const FEffectProperties& Props)
{

}

void UHawnmunAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if(!Props.SourceController && Props.SourceAvatarActor)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if(Props.SourceController)
		{	
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
