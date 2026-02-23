// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransitionInteractActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTH_API ATransitionInteractActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATransitionInteractActor();

private :
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> PlatformCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DestinationPoint;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> ViewDestinationPoint;
};
