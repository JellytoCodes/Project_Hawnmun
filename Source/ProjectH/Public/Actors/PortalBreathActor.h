// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalBreathActor.generated.h"

UCLASS()
class PROJECTH_API APortalBreathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalBreathActor();

	UFUNCTION(BlueprintCallable)
	void ActiveBreath(TArray<AActor*>& OutOverlappingActors);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActiveEffect();

	UFUNCTION(BlueprintCallable)
	void AddIgnoreActor(AActor* InActor);

private :
	UPROPERTY(EditDefaultsOnly)
	FVector BoxExtent;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
};
