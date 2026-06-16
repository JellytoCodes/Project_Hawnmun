// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HawnmunGameplayAbility.h"
#include "HawnmunProjectileAbility.generated.h"

class AHawnmunProjectileActor;

UCLASS()
class PROJECTH_API UHawnmunProjectileAbility : public UHawnmunGameplayAbility
{
	GENERATED_BODY()

protected :
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FName SocketName, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHawnmunProjectileActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 LoopSpawnProjectiles = 1;	
};
