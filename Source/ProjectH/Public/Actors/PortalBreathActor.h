// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "PortalBreathActor.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTH_API APortalBreathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalBreathActor();

	UFUNCTION(BlueprintCallable)
	void ActiveBreath(TArray<AActor*>& OutOverlappingActors, FDamageEffectParams CombatDamageEffectParams);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActiveEffect(const FDamageEffectParams& CombatDamageEffectParams);

	UFUNCTION(BlueprintCallable)
	void AddIgnoreActor(AActor* InActor);

private :
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> Effect;

	UPROPERTY(EditDefaultsOnly)
	FVector BoxExtent;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
};