// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunEnemy.h"
#include "HawnmunNineTailed.generated.h"

class APortalBreathActor;

UCLASS()
class PROJECTH_API AHawnmunNineTailed : public AHawnmunEnemy
{
	GENERATED_BODY()
	
public :
	AHawnmunNineTailed();

#pragma region CombatInterface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Die() override;
#pragma endregion

	UFUNCTION(BlueprintPure)
	APortalBreathActor* GetRandomPortalBreath();

protected :
	virtual void BeginPlay() override;


private:
	UPROPERTY(Transient)
	TArray<APortalBreathActor*> FieldPortalBreath;

};
