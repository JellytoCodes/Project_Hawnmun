// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "HawnmunFunctionLibrary.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Characters/HawnmunEnemy.h"

FDamageEffectParams UHawnmunGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FDamageEffectParams CauseDamageParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UHawnmunFunctionLibrary::ApplyDamageEffect(CauseDamageParams);

	return CauseDamageParams;
}

FDamageEffectParams UHawnmunGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage;
	Params.AbilityLevel = GetAbilityLevel();
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
