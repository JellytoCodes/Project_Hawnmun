// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HawnmunAbilitySystemComponent.generated.h"

class UHawnmunGameplayAbility;

// 타겟 디버깅 활성화 (Showdebug Abilitsystem 선택한 대상 정보 표시)
// [/Script/GameplayAbilities.AbilitySystemGlobals]
// bUseDebugTargetFromHud=true

UCLASS()
class PROJECTH_API UHawnmunAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	void AddCharacterActivateAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& ActivateAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UHawnmunGameplayAbility>>& PassiveAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InInputTag);
	void AbilityInputTagReleased(const FGameplayTag& InInputTag);
	void AbilityInputTagHeld(const FGameplayTag& InInputTag);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	bool InputTagIsEmpty(const FGameplayTag& InputTag);
	bool AbilityHasInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag);
	bool AbilityHasAnyInputTag(const FGameplayAbilitySpec& Spec);
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;

protected :
	
};
