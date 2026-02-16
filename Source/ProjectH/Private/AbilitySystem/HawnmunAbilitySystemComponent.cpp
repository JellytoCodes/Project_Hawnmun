// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"
#include "ProjectH/ProjectH.h"

void UHawnmunAbilitySystemComponent::AddCharacterActivateAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& ActivateAbilities)
{
	for (const auto& AbilityClass : ActivateAbilities)
	{
		const UHawnmunGameplayAbility* FatedBrandAbility = AbilityClass.GetDefaultObject();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if (FatedBrandAbility && FatedBrandAbility->StartupInputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FatedBrandAbility->StartupInputTag);
		}
		GiveAbility(AbilitySpec);
	}
}

void UHawnmunAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& PassiveAbilities)
{
	for (const auto& AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
	}
}

void UHawnmunAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && !IsPassiveAbility(AbilitySpec))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				const UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance();
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
				const FPredictionKey PredictionKey = Ability == nullptr ? AbilitySpec.ActivationInfo.GetActivationPredictionKey() : Ability->GetCurrentActivationInfo().GetActivationPredictionKey();
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, PredictionKey);
			}
		}
	}
}

void UHawnmunAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive() && !IsPassiveAbility(AbilitySpec))
		{
			AbilitySpecInputReleased(AbilitySpec);

			const UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance();
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			const FPredictionKey PredictionKey = Ability == nullptr ? AbilitySpec.ActivationInfo.GetActivationPredictionKey() : Ability->GetCurrentActivationInfo().GetActivationPredictionKey();
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, PredictionKey);
		}
	}
}

void UHawnmunAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && !IsPassiveAbility(AbilitySpec))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

FGameplayTag UHawnmunAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UHawnmunAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UHawnmunAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

bool UHawnmunAbilitySystemComponent::InputTagIsEmpty(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(AbilitySpec, InputTag)) return false;
	}
	return true;
}

bool UHawnmunAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag);
}

bool UHawnmunAbilitySystemComponent::AbilityHasAnyInputTag(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

bool UHawnmunAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	//const UDataAsset_AbilityInfo* AbilityInfo = UFatedBrandFunctionLibrary::GetAbilityInfo(GetAvatarActor());
	//const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	//const FFatedBrandAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	//const FGameplayTag AbilityType = Info.AbilityType;
	//
	//return AbilityType.MatchesTagExact(FatedBrandGameplayTags::Abilities_Passive_Type);

	return false;
}