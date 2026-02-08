// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "HawnmunEnemy.generated.h"

UCLASS()
class PROJECTH_API AHawnmunEnemy : public AHawnmunCharacterBase
{
	GENERATED_BODY()

public :
	AHawnmunEnemy();

protected :
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};
