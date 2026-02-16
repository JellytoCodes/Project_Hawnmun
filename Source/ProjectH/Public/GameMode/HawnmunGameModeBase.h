// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HawnmunGameModeBase.generated.h"

class UDataAsset_AbilityInfo;

UCLASS()
class PROJECTH_API AHawnmunGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfo;

protected :
	virtual void BeginPlay() override;
};
