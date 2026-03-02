// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HawnmunEnemy.h"
#include "HawnmunNineTailed.generated.h"

class AHawnmunNineTailedIllusion;
class UNiagaraComponent;
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActivateCharmIllusionEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeactivateCharmIllusionEffect();

	UFUNCTION(BlueprintCallable)
	void ActivateIllusionsEffect();

	UFUNCTION(BlueprintCallable)
	void DeactivateIllusionsEffect();

	UFUNCTION(BlueprintCallable)
	void ShuffleNineTailedCharmIllusion();

	UFUNCTION(BlueprintCallable)
	void BackCenterLocation(const FDamageEffectParams& DamageEffectParams);

protected :
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<APortalBreathActor>> FieldPortalBreath;

	void CreatePortalBreath();

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TSoftClassPtr<AFoxFireDropActor> SpawnFoxFireDropClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AFoxFireDropActor>> FoxFireCardinalDrop;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AFoxFireDropActor>> FoxFireDiagonalDrop;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TArray<FVector> CardinalDropLocation;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFireDrop")
	TArray<FVector> DiagonalDropLocation;

	void CreateFoxFireDrops();

	UPROPERTY(EditDefaultsOnly, Category = "NineTailedIllusion")
	TSoftClassPtr<AHawnmunNineTailedIllusion> SpawnNineTailedIllusionClass;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AHawnmunNineTailedIllusion>> NineTailedIllusion;

	UPROPERTY()
	TArray<FVector> NineTailedIllusionLocation;

	void CreateNineTailedIllusion();
	void GenerateCirclePoints(float Radius);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> CharmIllusionEffect;

	FVector CachedCenterLocation;
};
