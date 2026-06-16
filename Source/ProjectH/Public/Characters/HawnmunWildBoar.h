// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunEnemy.h"
#include "HawnmunWildBoar.generated.h"

class ATargetPoint;

UCLASS()
class PROJECTH_API AHawnmunWildBoar : public AHawnmunEnemy
{
	GENERATED_BODY()
	
public:

#pragma region CombatInterface
	virtual bool TryPhaseTransition() override;
	virtual void Die() override;
	virtual void ResumeEnemyAI_Implementation() override;
#pragma endregion

private:
	void StopWildBoarCombatLogic(const FString& Reason);

	UPROPERTY(EditInstanceOnly, Category = "WildBoar|Defeat")
	TObjectPtr<ATargetPoint> DefeatTransitionPoint;

	void MovePlayerToDefeatTransitionPoint() const;
};
