// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#include "AbilitySystem/DataAsset_AbilityInfo.h"

FHawnmunAbilityInfo UDataAsset_AbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const
{
	for (const FHawnmunAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	return FHawnmunAbilityInfo();
}
