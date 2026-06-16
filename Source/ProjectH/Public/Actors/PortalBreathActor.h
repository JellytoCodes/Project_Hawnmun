// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "PortalBreathActor.generated.h"

class UNiagaraComponent;
class USceneComponent;
class UDecalComponent;
class UMaterialInstanceDynamic;

UCLASS()
class PROJECTH_API APortalBreathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalBreathActor();

	UFUNCTION(BlueprintCallable)
	void ActivePortalBreath(const FDamageEffectParams& CombatDamageEffectParams);

	UFUNCTION(BlueprintCallable)
	void AddIgnoreActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "PortalBreath")
	void UpdateWarningDecalProgress(float Progress);

	UFUNCTION(BlueprintCallable, Category = "PortalBreath")
	void FireBeam();

	UFUNCTION(BlueprintCallable)
	void CancelPortalBreath();

protected:
	virtual void BeginPlay() override;
	void ApplyDamage();

	UFUNCTION(BlueprintImplementableEvent, Category = "PortalBreath")
	void StartChargeAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "PortalBreath")
	void OnBeamFired();

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalBreath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalBreath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> WarningDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalBreath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> PortalEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalBreath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> BeamEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalBreath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> FoxHeadEffect;

	UPROPERTY(EditDefaultsOnly, Category = "PortalBreath")
	FVector BoxExtent = FVector(200.f, 120.f, 100.f);

	UPROPERTY(Transient)
	FDamageEffectParams CachedDamageParams;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DecalMID;

	bool bIsActive = false;
};