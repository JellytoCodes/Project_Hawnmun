// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/HawnmunAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HawnmunGameplayTags.h"
#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"
#include "GameMode/HawnmunPlayerState.h"
#include "Interfaces/PlayerInterface.h"
#include "ProjectH/ProjectH.h"

#pragma region Initialization And Setup
void UHawnmunAbilitySystemComponent::AddCharacterActivateAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& ActivateAbilities)
{
	for (const auto& AbilityClass : ActivateAbilities)
	{
		if (!AbilityClass) continue;

		bool bAlreadyGranted = false;
		for (const FGameplayAbilitySpec& ExistingSpec : GetActivatableAbilities())
		{
			if (ExistingSpec.Ability && ExistingSpec.Ability->GetClass() == AbilityClass)
			{
				bAlreadyGranted = true;
				break;
			}
		}

		if (bAlreadyGranted)
		{
			continue;
		}

		const UHawnmunGameplayAbility* HawnmunAbility = AbilityClass.GetDefaultObject();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (HawnmunAbility && HawnmunAbility->StartupInputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(HawnmunAbility->StartupInputTag);
		}

		GiveAbility(AbilitySpec);
	}
}

void UHawnmunAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& PassiveAbilities)
{
	for (const auto& AbilityClass : PassiveAbilities)
	{
		if (!AbilityClass) continue;

		bool bAlreadyGranted = false;
		for (const FGameplayAbilitySpec& ExistingSpec : GetActivatableAbilities())
		{
			if (ExistingSpec.Ability && ExistingSpec.Ability->GetClass() == AbilityClass)
			{
				bAlreadyGranted = true;
				break;
			}
		}

		if (bAlreadyGranted)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}
#pragma endregion

#pragma region Input Handling
void UHawnmunAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (AbilitySpec.IsActive() && InInputTag.MatchesTag(HawnmunGameplayTags::Input_Key_MButton))
			{
				CancelAbilityHandle(AbilitySpec.Handle);
				return;
			}

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
		if (InInputTag.MatchesTag(HawnmunGameplayTags::Input_Key_MButton) && AbilitySpec.IsActive())
		{
			return;
		}

		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
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
#pragma endregion

#pragma region Attribute Management
void UHawnmunAbilitySystemComponent::ApplyAttributeUpgrades(const TMap<FGameplayTag, int32>& Upgrades)
{
	int32 TotalCost = 0;
	for (const auto& Pair : Upgrades) TotalCost += Pair.Value;

	if (TotalCost == 0) return;

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		AHawnmunPlayerState* PS = Cast<AHawnmunPlayerState>(IPlayerInterface::Execute_GetHawnmunPS(GetAvatarActor()));

		if (PS && PS->GetAttributePoints() >= TotalCost)
		{
			PS->AddToAttributePoints(-TotalCost);

			for (const auto& Pair : Upgrades)
			{
				if (Pair.Value > 0)
				{
					FGameplayEventData Payload;
					Payload.EventTag = Pair.Key;
					Payload.EventMagnitude = Pair.Value;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), Pair.Key, Payload);
				}
			}
		}
	}
}
#pragma endregion