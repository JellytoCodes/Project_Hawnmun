// Copyright (c) 2026 Project Hawnmun. All rights reserved.


#include "AbilitySystem/DataAsset_AttributeInfo.h"

FAttributeInfo UDataAsset_AttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag) const
{
	for (const FAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}
	return FAttributeInfo();
}
