// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTH_API IEnemyInterface
{
	GENERATED_BODY()

public:
	virtual bool TryPhaseTransition() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void ResumeEnemyAI();

	UFUNCTION(BlueprintNativeEvent)
	void SetAggroTarget(AActor* TargetActor);

};
