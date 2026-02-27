// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "FoxFireDropActor.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTH_API AFoxFireDropActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoxFireDropActor();

	UFUNCTION(BlueprintCallable)
	void ActiveFoxFireDrop(TArray<AActor*>& OutOverlappingActors, FDamageEffectParams CombatDamageEffectParams);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActiveEffect(const FDamageEffectParams& CombatDamageEffectParams);

	UFUNCTION(BlueprintCallable)
	void AddIgnoreActor(AActor* InActor);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> Effect;

	UPROPERTY(EditDefaultsOnly)
	float Radius;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
};
