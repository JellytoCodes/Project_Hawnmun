// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "HawnmunTriggerBase.generated.h"

class UBoxComponent;

UCLASS(Abstract)
class PROJECTH_API AHawnmunTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AHawnmunTriggerBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	UFUNCTION(BlueprintCallable, Category = "Hawnmun|Trigger")
	void ExecuteCinematicRequest(FGameplayTag CinematicTag, AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Hawnmun|Trigger")
	void SaveCheckpointFromActor(AActor* InActor) const;
};