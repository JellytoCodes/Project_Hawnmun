// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Types/HawnmunStructTypes.h"
#include "DataAsset_AbilityInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDataAsset_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FHawnmunAbilityInfo> AbilityInformation;

	FHawnmunAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const;
};
