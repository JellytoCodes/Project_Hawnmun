// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "Input/DataAsset_InputConfig.h"

const UInputAction* UDataAsset_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for (const FHawnmunInputAction& Action : AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag == InputTag)	return Action.InputAction;
	}

	if (bLogNotFound)	
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for Input Tag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}
