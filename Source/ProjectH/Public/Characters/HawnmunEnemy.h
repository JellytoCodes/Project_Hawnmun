// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "HawnmunEnemy.generated.h"

class AHawnmunAIController;
class UBehaviorTree;

UCLASS()
class PROJECTH_API AHawnmunEnemy : public AHawnmunCharacterBase
{
	GENERATED_BODY()

public :
	AHawnmunEnemy();

	void SetSpawnedActorByBlackBoardKey(AActor* spawnedActor);

protected :
	virtual void InitAbilityActorInfo() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHawnmunAIController> HawnmunAIController;
};
