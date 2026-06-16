// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class USceneComponent;
class UBillboardComponent;

UCLASS()
class PROJECTH_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSoftClassPtr<AActor> SpawnClass;

	UPROPERTY(SaveGame)
	bool bReached = false;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	bool bUsePersistence = true;

	UPROPERTY()
	TWeakObjectPtr<AActor> Spawned;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Scene;

protected :
	virtual void BeginPlay() override;

private :
	UFUNCTION()
	void OnSpawnedDestroyed(AActor* DestroyedActor);

};
