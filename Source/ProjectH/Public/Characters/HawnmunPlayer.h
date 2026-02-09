// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunCharacterBase.h"
#include "HawnmunPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTH_API AHawnmunPlayer : public AHawnmunCharacterBase
{
	GENERATED_BODY()

public :
	AHawnmunPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

private :
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
};
