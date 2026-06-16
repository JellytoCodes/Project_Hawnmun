// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/HawnmunPlayer.h"
#include "HawnmunFunctionLibrary.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Characters/HawnmunEnemy.h"
#include "Components/EquipmentComponent.h"

FDamageEffectParams UHawnmunGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FDamageEffectParams CauseDamageParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UHawnmunFunctionLibrary::ApplyDamageEffect(CauseDamageParams);

	return CauseDamageParams;
}

FDamageEffectParams UHawnmunGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor)
{
	FDamageEffectParams Params;
	
	checkf(DamageEffectClass, TEXT("MakeDamageEffectParams failed: DamageEffectClass is null on ability %s."), *GetNameSafe(this));
	checkf(DamageType.IsValid(), TEXT("MakeDamageEffectParams failed: DamageType is invalid on ability %s."), *GetNameSafe(this));

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	checkf(SourceASC, TEXT("MakeDamageEffectParams failed: Source ASC is null on ability %s."), *GetNameSafe(this));

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	Params.SourceAbilitySystemComponent = SourceASC;
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityLevel = GetAbilityLevel();

	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = RadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;	
	}

	float TotalDamage = Damage;

	if (AHawnmunPlayer* Player = GetHawnmunCharacterFromActorInfo())
	{
		if (UEquipmentComponent* EquipComp = Player->GetEquipmentComponent()) 
		{
			if (Params.SourceAbilitySystemComponent->HasMatchingGameplayTag(HawnmunGameplayTags::Event_Equip_HasMainWeapon))
			{
				TotalDamage += EquipComp->GetEquippedItemBaseDamage(HawnmunGameplayTags::EquipSlot_Weapon);
			}
			else
			{
				TotalDamage += EquipComp->GetEquippedItemBaseDamage(HawnmunGameplayTags::EquipSlot_Ammo);
			}
		}
	}
	Params.BaseDamage = TotalDamage;

	return Params;
}

void UHawnmunGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHawnmunGameplayAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

UHawnmunAbilitySystemComponent* UHawnmunGameplayAbility::GetHawnmunAbilitySystemComponentFromActorInfo() const
{
	return Cast<UHawnmunAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

AHawnmunPlayer* UHawnmunGameplayAbility::GetHawnmunCharacterFromActorInfo()
{
	if (!CachedHawnmunCharacter.IsValid())
	{
		CachedHawnmunCharacter = Cast<AHawnmunPlayer>(CurrentActorInfo->AvatarActor);
	}
	return CachedHawnmunCharacter.IsValid() ? CachedHawnmunCharacter.Get() : nullptr;
}

AHawnmunEnemy* UHawnmunGameplayAbility::GetHawnmunEnemyFromActorInfo()
{
	if (!CachedHawnmunEnemy.IsValid())
	{
		CachedHawnmunEnemy = Cast<AHawnmunEnemy>(CurrentActorInfo->AvatarActor);
	}
	return CachedHawnmunEnemy.IsValid() ? CachedHawnmunEnemy.Get() : nullptr;
}
