// Copyright (c) 2026 Project Hawnmun. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/HawnmunStructTypes.h"
#include "PortalBiteActor.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTH_API APortalBiteActor : public AActor
{
	GENERATED_BODY()

public:	
	APortalBiteActor();

	void ActivatePortalBite(const FVector& TargetLocation, const FDamageEffectParams& DamageParams);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> TelegraphEffect;

	UPROPERTY(VisibleDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> BiteEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDelay = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DamageRadius = 350.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> BiteSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> CastSound;

	UPROPERTY(Transient)
	FDamageEffectParams CachedDamageParams;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	void ApplyDamage();

	void AddIgnoreActor(AActor* InActor);
};
