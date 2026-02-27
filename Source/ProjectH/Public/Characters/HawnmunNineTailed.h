// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunEnemy.h"
#include "HawnmunNineTailed.generated.h"

class UNiagaraSystem;
class AFoxFireVolleyActor;
class AFoxFireDropActor;
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

	UFUNCTION(BlueprintCallable)
	void ActiveFoxFireCardinalDrop(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable)
	void ActiveFoxFireDiagonalDrop(const FDamageEffectParams& DamageEffectParams);

protected :
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<APortalBreathActor>> FieldPortalBreath;

	void CreatePortalBreath();

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TSoftClassPtr<AFoxFireDropActor> SpawnClass;

	UPROPERTY(Transient)
	TArray< TObjectPtr<AFoxFireDropActor>> FoxFireCardinalDrop;

	UPROPERTY(Transient)
	TArray< TObjectPtr<AFoxFireDropActor>> FoxFireDiagonalDrop;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TArray<FVector> CardinalDrop;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TArray<FVector> DiagonalDrop;

	void CreateFoxFireDrops();

};
