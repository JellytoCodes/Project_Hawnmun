// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"
#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "Characters/HawnmunPlayer.h"
#include "Characters/HawnmunEnemy.h"

FDamageEffectParams UHawnmunGameplayAbility::CauseDamage(AActor* TargetActor)
{
	// TODO : 데미지 적용 구현
	return FDamageEffectParams();
}

FDamageEffectParams UHawnmunGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	// TODO : 데미지 파라미터 구현
	return FDamageEffectParams();
}

void UHawnmunGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
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

UHawnmunAbilitySystemComponent* UHawnmunGameplayAbility::GetFatedBrandAbilitySystemComponentFromActorInfo() const
{
	return Cast<UHawnmunAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

AHawnmunPlayer* UHawnmunGameplayAbility::GetFatedBrandCharacterFromActorInfo()
{
	if (!CachedFatedBrandCharacter.IsValid())
	{
		CachedFatedBrandCharacter = Cast<AHawnmunPlayer>(CurrentActorInfo->AvatarActor);
	}
	return CachedFatedBrandCharacter.IsValid() ? CachedFatedBrandCharacter.Get() : nullptr;
}

AHawnmunEnemy* UHawnmunGameplayAbility::GetFatedBrandEnemyFromActorInfo()
{
	if (!CachedFatedBrandEnemy.IsValid())
	{
		CachedFatedBrandEnemy = Cast<AHawnmunEnemy>(CurrentActorInfo->AvatarActor);
	}
	return CachedFatedBrandEnemy.IsValid() ? CachedFatedBrandEnemy.Get() : nullptr;
}
