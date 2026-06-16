// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "FoxFireDropActor.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
class USceneComponent;

UCLASS()
class PROJECTH_API AFoxFireDropActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoxFireDropActor();

	UFUNCTION(BlueprintCallable)
	void ActiveFoxFireDrop(const FDamageEffectParams& CombatDamageEffectParams);

	UFUNCTION(BlueprintCallable)
	void AddIgnoreActor(AActor* InActor);

protected:
	virtual void BeginPlay() override;

	void ApplyDamage();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "FoxFire")
	void OnImpact(const FVector& ImpactLocation);

	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> WarningEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> ProjectileEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> ExplosionEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> FallingCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FoxFire", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> DropSound;

	UPROPERTY(VisibleAnywhere, Category = "FoxFire")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFire")
	float Radius = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFire")
	float FallingDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "FoxFire")
	float SpawnHeightOffset = 2000.f;

	UPROPERTY(Transient)
	FDamageEffectParams CachedDamageParams;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
};